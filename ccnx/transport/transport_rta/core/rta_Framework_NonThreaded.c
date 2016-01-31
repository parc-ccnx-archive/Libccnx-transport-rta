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
/**
 *  *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */

#include <config.h>
#include <stdio.h>
#include <unistd.h>

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

// This is implemented in rta_Framework_Commands
void
rtaFramework_DestroyProtocolHolder(RtaFramework *framework, FrameworkProtocolHolder *holder);

/**
 * If running in non-threaded mode (you don't call _Start), you must manually
 * turn the crank.  This turns it for a single cycle.
 * Return 0 on success, -1 on error (likely you're running in threaded mode)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaFramework_NonThreadedStep(RtaFramework *framework)
{
    if (framework->status == FRAMEWORK_INIT) {
        framework->status = FRAMEWORK_SETUP;
    }

    assertTrue(framework->status == FRAMEWORK_SETUP,
               "Framework invalid state for non-threaded, expected %d got %d",
               FRAMEWORK_SETUP,
               framework->status
               );

    if (framework->status != FRAMEWORK_SETUP) {
        return -1;
    }

    if (parcEventScheduler_Start(framework->base, PARCEventSchedulerDispatchType_LoopOnce) < 0) {
        return -1;
    }

    return 0;
}

/**
 * If running in non-threaded mode (you don't call _Start), you must manually
 * turn the crank.  This turns it for a number of cycles.
 * Return 0 on success, -1 on error (likely you're running in threaded mode)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaFramework_NonThreadedStepCount(RtaFramework *framework, unsigned count)
{
    if (framework->status == FRAMEWORK_INIT) {
        framework->status = FRAMEWORK_SETUP;
    }

    assertTrue(framework->status == FRAMEWORK_SETUP,
               "Framework invalid state for non-threaded, expected %d got %d",
               FRAMEWORK_SETUP,
               framework->status
               );

    if (framework->status != FRAMEWORK_SETUP) {
        return -1;
    }

    while (count-- > 0) {
        if (parcEventScheduler_Start(framework->base, PARCEventSchedulerDispatchType_LoopOnce) < 0) {
            return -1;
        }
    }
    return 0;
}

/**
 * If running in non-threaded mode (you don't call _Start), you must manually
 * turn the crank.  This turns it for a given amount of time.
 * Return 0 on success, -1 on error (likely you're running in threaded mode)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaFramework_NonThreadedStepTimed(RtaFramework *framework, struct timeval *duration)
{
    if (framework->status == FRAMEWORK_INIT) {
        framework->status = FRAMEWORK_SETUP;
    }

    assertTrue(framework->status == FRAMEWORK_SETUP,
               "Framework invalid state for non-threaded, expected %d got %d",
               FRAMEWORK_SETUP,
               framework->status
               );

    if (framework->status != FRAMEWORK_SETUP) {
        return -1;
    }

    parcEventScheduler_Stop(framework->base, duration);

    if (parcEventScheduler_Start(framework->base, 0) < 0) {
        return -1;
    }
    return 0;
}


/**
 * After a protocol stack is created, you need to Teardown.  If you
 * are running in threaded mode (did a _Start), you should send an asynchronous
 * SHUTDOWN command instead.  This function only works if in the SETUP state
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaFramework_Teardown(RtaFramework *framework)
{
    FrameworkProtocolHolder *holder;

    assertNotNull(framework, "called with null framework");

    if (DEBUG_OUTPUT) {
        printf("%9" PRIu64 " %s framework %p\n",
               rtaFramework_GetTicks(framework),
               __func__, (void *) framework);
    }

    // %%% LOCK
    rta_Framework_LockStatus(framework);
    if (framework->status != FRAMEWORK_SETUP) {
        RtaFrameworkStatus status = framework->status;
        rta_Framework_UnlockStatus(framework);
        // %%% UNLOCK
        assertTrue(0, "Invalid state, expected FRAMEWORK_SETUP, got %d", status);
        return -1;
    }

    holder = TAILQ_FIRST(&framework->protocols_head);
    while (holder != NULL) {
        FrameworkProtocolHolder *temp = TAILQ_NEXT(holder, list);
        rtaFramework_DestroyProtocolHolder(framework, holder);
        holder = temp;
    }

    framework->status = FRAMEWORK_TEARDOWN;
    rta_Framework_BroadcastStatus(framework);
    rta_Framework_UnlockStatus(framework);
    // %%% UNLOCK

    return 0;
}
