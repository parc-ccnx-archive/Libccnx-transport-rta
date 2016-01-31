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
 * @file component_Testing.h
 * @brief A component that takes no actions, not even reads.
 *
 * This is useful to put in the stack around a component under test to
 * isolate it from the system and then intercept the queues.
 *
 * This component may be used as both TESTING_UPPER and TESTING_LOWER
 * to surround another component:
 *
 * { SYSTEM : COMPONENTS : [TESTING_UPPER, component under test, TESTING_LOWER] }
 *
 * In your test code, you would then have something like this to operate in
 * the "down" direction:
 *    PARCEventQueue *upper = rtaProtocolStack_GetPutQueue(stack, TESTING_UPPER, RTA_DOWN);
 *    PARCEventQueue *rea   = rtaProtocolStack_GetPutQueue(stack, component under test, RTA_UP);
 *    PARCEventQueue *lower = rtaProtocolStack_GetPutQueue(stack, TESTING_LOWER, RTA_UP);
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_component_Testing_h
#define Libccnx_component_Testing_h

#include <ccnx/transport/transport_rta/core/rta_Component.h>

extern RtaComponentOperations testing_null_ops;
#endif // Libccnx_component_Testing_h
