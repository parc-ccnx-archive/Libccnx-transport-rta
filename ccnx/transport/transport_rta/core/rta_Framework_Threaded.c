/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 *
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
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
