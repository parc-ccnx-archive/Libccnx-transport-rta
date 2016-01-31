/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>

#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>

#include "rta_Framework.h"
#include "rta_ConnectionTable.h"
#include "rta_Framework_Commands.h"

#ifndef DEBUG_OUTPUT
#define DEBUG_OUTPUT 0
#endif

// the thread function
static void *_rtaFramework_Run(void *ctx);

/**
 * Starts the worker thread.  Blocks until started
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
rtaFramework_Start(RtaFramework *framework)
{
    pthread_attr_t attr;

    // ensure we're in the INIT state, then bump to STARTING
    // %%% LOCK
    rta_Framework_LockStatus(framework);
    if (framework->status == FRAMEWORK_INIT) {
        framework->status = FRAMEWORK_STARTING;
        rta_Framework_BroadcastStatus(framework);
        rta_Framework_UnlockStatus(framework);
        // %%% UNLOCK
    } else {
        RtaFrameworkStatus status = framework->status;
        rta_Framework_UnlockStatus(framework);
        // %%% UNLOCK
        assertTrue(0, "Invalid state, not FRAMEWORK_INIT, got %d", status);
        return;
    }


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (pthread_create(&framework->thread, &attr, _rtaFramework_Run, framework) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (DEBUG_OUTPUT) {
        printf("%s framework started %p\n", __func__, (void *) framework);
    }

    // wait for notificaiton from event thread
    rta_Framework_LockStatus(framework);
    while (framework->status == FRAMEWORK_INIT) {
        rta_Framework_WaitStatus(framework);
    }
    rta_Framework_UnlockStatus(framework);

    if (DEBUG_OUTPUT) {
        printf("%s framework running %p\n", __func__, (void *) framework);
    }
}

static void *
_rtaFramework_Run(void *ctx)
{
    RtaFramework *framework = (RtaFramework *) ctx;

    // %%% LOCK
    rta_Framework_LockStatus(framework);
    if (framework->status != FRAMEWORK_STARTING) {
        assertTrue(0, "Invalid state, expected before %d, got %d", FRAMEWORK_STARTING, framework->status);
        rta_Framework_UnlockStatus(framework);
        // %%% UNLOCK
        pthread_exit(NULL);
    }
    framework->status = FRAMEWORK_RUNNING;

    // Set our thread name, only used to diagnose a crash or in debugging
#if __APPLE__
    pthread_setname_np("RTA Framework");
#else
    pthread_setname_np(framework->thread, "RTA Framework");
#endif

    rta_Framework_BroadcastStatus(framework);
    rta_Framework_UnlockStatus(framework);
    // %%% UNLOCK

    if (DEBUG_OUTPUT) {
        const int bufferLength = 1024;
        char frameworkName[bufferLength];
        pthread_getname_np(framework->thread, frameworkName, bufferLength);
        printf("Framework thread running: '%s'\n", frameworkName);
    }

    // blocks
    parcEventScheduler_Start(framework->base, PARCEventSchedulerDispatchType_Blocking);

    if (DEBUG_OUTPUT) {
        printf("%9" PRIu64 " %s existed parcEventScheduler_Start\n", framework->clock_ticks, __func__);
    }

    // %%% LOCK
    rta_Framework_LockStatus(framework);
    framework->status = FRAMEWORK_SHUTDOWN;
    rta_Framework_BroadcastStatus(framework);
    rta_Framework_UnlockStatus(framework);
    // %%% UNLOCK

    pthread_exit(NULL);
}

/**
 * Stops the worker thread by sending a CommandShutdown.
 * Blocks until shutdown complete.
 *
 * CALLED FROM API's THREAD
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
rtaFramework_Shutdown(RtaFramework *framework)
{
    RtaCommand *shutdown = rtaCommand_CreateShutdownFramework();
    rtaCommand_Write(shutdown, framework->commandRingBuffer);
    parcNotifier_Notify(framework->commandNotifier);
    rtaCommand_Release(&shutdown);

    // now block on reading status
    rtaFramework_WaitForStatus(framework, FRAMEWORK_SHUTDOWN);
}
