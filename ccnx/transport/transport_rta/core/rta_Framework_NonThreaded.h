/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file rta_Framework_NonThreaded.h
 * @brief Implementation of the non-threaded api.
 *
 * Unless you call one of the _Step methods frequently, the tick clock will be off.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_Framework_NonThreaded_h
#define Libccnx_rta_Framework_NonThreaded_h

#include <sys/time.h>

// ==============================
// NON-THREADED API

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
int rtaFramework_NonThreadedStep(RtaFramework *framework);

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
int rtaFramework_NonThreadedStepCount(RtaFramework *framework, unsigned count);

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
int rtaFramework_NonThreadedStepTimed(RtaFramework *framework, struct timeval *duration);


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
int rtaFramework_Teardown(RtaFramework *framework);
#endif
