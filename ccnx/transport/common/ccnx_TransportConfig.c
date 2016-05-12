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
#include <parc/algol/parc_Memory.h>
#include <LongBow/runtime.h>

#include <ccnx/transport/common/ccnx_TransportConfig.h>

struct transport_config {
    CCNxStackConfig *stackConfig;
    CCNxConnectionConfig *connConfig;
};

bool
ccnxTransportConfig_IsValid(const CCNxTransportConfig *transportConfig)
{
    bool result = false;

    // should test that the configuration is valid, not just null (case 890)
    if (transportConfig != NULL) {
        if (ccnxStackConfig_IsValid(transportConfig->stackConfig)) {
            if (ccnxConnectionConfig_IsValid(transportConfig->connConfig)) {
                result = true;
            }
        }
    }
    return result;
}

void
ccnxTransportConfig_AssertValid(const CCNxTransportConfig *config)
{
    assertTrue(ccnxTransportConfig_IsValid(config), "CCNxTransportConfig instance is invalid.");
}

CCNxTransportConfig *
ccnxTransportConfig_Create(CCNxStackConfig *stackConfig, CCNxConnectionConfig *connConfig)
{
    ccnxStackConfig_OptionalAssertValid(stackConfig);
    ccnxConnectionConfig_OptionalAssertValid(connConfig);

    CCNxTransportConfig *result = parcMemory_AllocateAndClear(sizeof(CCNxTransportConfig));
    assertNotNull(result, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CCNxTransportConfig));
    result->stackConfig = ccnxStackConfig_Acquire(stackConfig);
    result->connConfig = connConfig;
    return result;
}

void
ccnxTransportConfig_Destroy(CCNxTransportConfig **transportConfigPtr)
{
    assertNotNull(transportConfigPtr, "Parameter must be non-null double pointer");
    ccnxTransportConfig_OptionalAssertValid(*transportConfigPtr);

    CCNxTransportConfig *transConfig = *transportConfigPtr;
    ccnxStackConfig_Release(&transConfig->stackConfig);
    ccnxConnectionConfig_Destroy(&transConfig->connConfig);
    parcMemory_Deallocate((void **) &transConfig);
    *transportConfigPtr = NULL;
}

CCNxStackConfig *
ccnxTransportConfig_GetStackConfig(const CCNxTransportConfig *transportConfig)
{
    ccnxTransportConfig_OptionalAssertValid(transportConfig);

    return transportConfig->stackConfig;
}

CCNxConnectionConfig *
ccnxTransportConfig_GetConnectionConfig(const CCNxTransportConfig *transportConfig)
{
    ccnxTransportConfig_OptionalAssertValid(transportConfig);

    return transportConfig->connConfig;
}

bool
ccnxTransportConfig_Equals(const CCNxTransportConfig *x, const CCNxTransportConfig *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (ccnxStackConfig_Equals(x->stackConfig, y->stackConfig)) {
            result = ccnxConnectionConfig_Equals(x->connConfig, y->connConfig);
        }
    }

    return result;
}

CCNxTransportConfig *
ccnxTransportConfig_Copy(const CCNxTransportConfig *original)
{
    ccnxTransportConfig_OptionalAssertValid(original);

    CCNxTransportConfig *copy = parcMemory_AllocateAndClear(sizeof(CCNxTransportConfig));
    assertNotNull(copy, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CCNxTransportConfig));

    copy->stackConfig = ccnxStackConfig_Copy(original->stackConfig);
    copy->connConfig = ccnxConnectionConfig_Copy(original->connConfig);
    return copy;
}
