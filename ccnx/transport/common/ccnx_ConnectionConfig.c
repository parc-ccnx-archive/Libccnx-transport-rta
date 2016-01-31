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
 * These are subsystems instantiated within components
 * They define per-connection behavior, not stack structure.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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
