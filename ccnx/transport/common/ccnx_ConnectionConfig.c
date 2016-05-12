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
 * These are subsystems instantiated within components
 * They define per-connection behavior, not stack structure.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <ccnx/transport/common/ccnx_TransportConfig.h>

struct ccnx_connection_config {
    PARCJSON *connjson;
};

bool
ccnxConnectionConfig_IsValid(const CCNxConnectionConfig *config)
{
    bool result = false;
    if (config != NULL) {
        result = true;
    }
    return result;
}

void
ccnxConnectionConfig_AssertValid(const CCNxConnectionConfig *config)
{
    assertTrue(ccnxConnectionConfig_IsValid(config), "CCNxConnectionConfig instance is invalid.");
}

CCNxConnectionConfig *
ccnxConnectionConfig_Create(void)
{
    CCNxConnectionConfig *config = parcMemory_AllocateAndClear(sizeof(CCNxConnectionConfig));
    assertNotNull(config, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CCNxConnectionConfig));
    config->connjson = parcJSON_Create();
    return config;
}

void
ccnxConnectionConfig_Destroy(CCNxConnectionConfig **connectionConfigPtr)
{
    assertNotNull(connectionConfigPtr, "Parameter must be non-null double pointer");

    CCNxConnectionConfig *config = *connectionConfigPtr;
    ccnxConnectionConfig_OptionalAssertValid(config);

    parcJSON_Release(&config->connjson);
    parcMemory_Deallocate((void **) &config);
    *connectionConfigPtr = NULL;
}

PARCJSON *
ccnxConnectionConfig_GetJson(const CCNxConnectionConfig *config)
{
    ccnxConnectionConfig_OptionalAssertValid(config);

    return config->connjson;
}

CCNxConnectionConfig *
ccnxConnectionConfig_Add(CCNxConnectionConfig *config, const char *key, PARCJSONValue *componentJson)
{
    ccnxConnectionConfig_OptionalAssertValid(config);

    parcJSON_AddValue(config->connjson, key, componentJson);
    return config;
}

CCNxConnectionConfig *
ccnxConnectionConfig_Copy(const CCNxConnectionConfig *original)
{
    ccnxConnectionConfig_OptionalAssertValid(original);

    CCNxConnectionConfig *copy = parcMemory_AllocateAndClear(sizeof(CCNxConnectionConfig));
    assertNotNull(copy, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CCNxConnectionConfig));
    copy->connjson = parcJSON_Copy(original->connjson);
    return copy;
}

bool
ccnxConnectionConfig_Equals(const CCNxConnectionConfig *x, const CCNxConnectionConfig *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        result = parcJSON_Equals(x->connjson, y->connjson);
    }

    return result;
}

void
ccnxConnectionConfig_Display(const CCNxConnectionConfig *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "ConnectionConfig@%p {", instance);
    PARCJSON *json = ccnxConnectionConfig_GetJson(instance);

    parcJSON_Display(json, indentation + 1);
    parcDisplayIndented_PrintLine(indentation, "}");
}
