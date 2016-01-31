/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <ccnx/transport/common/ccnx_StackConfig.h>

struct CCNxStackConfig_ {
    PARCJSON *stackjson;
};

static void
_ccnxStackConfig_Finalize(CCNxStackConfig **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a CCNxStackConfig pointer.");

    CCNxStackConfig *instance = *instancePtr;
    ccnxStackConfig_OptionalAssertValid(instance);

    parcJSON_Release(&instance->stackjson);
}

parcObject_ImplementAcquire(ccnxStackConfig, CCNxStackConfig);

parcObject_ImplementRelease(ccnxStackConfig, CCNxStackConfig);

parcObject_ExtendPARCObject(CCNxStackConfig, _ccnxStackConfig_Finalize, ccnxStackConfig_Copy, ccnxStackConfig_ToString, ccnxStackConfig_Equals, NULL, ccnxStackConfig_HashCode, ccnxStackConfig_ToJSON);

void
ccnxStackConfig_AssertValid(const CCNxStackConfig *instance)
{
    assertTrue(ccnxStackConfig_IsValid(instance),
               "CCNxStackConfig is not valid.");
}

CCNxStackConfig *
ccnxStackConfig_Create(void)
{
    CCNxStackConfig *result = parcObject_CreateInstance(CCNxStackConfig);
    if (result != NULL) {
        result->stackjson = parcJSON_Create();
    }

    return result;
}

CCNxStackConfig *
ccnxStackConfig_Copy(const CCNxStackConfig *original)
{
    ccnxStackConfig_OptionalAssertValid(original);

    CCNxStackConfig *result = parcObject_CreateInstance(CCNxStackConfig);

    result->stackjson = parcJSON_Copy(original->stackjson);

    return result;
}

void
ccnxStackConfig_Display(const CCNxStackConfig *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "CCNxStackConfig@%p {", instance);
    PARCJSON *json = ccnxStackConfig_GetJson(instance);

    parcJSON_Display(json, indentation + 1);
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
ccnxStackConfig_Equals(const CCNxStackConfig *x, const CCNxStackConfig *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        result = parcJSON_Equals(x->stackjson, y->stackjson);
    }

    return result;
}

bool
ccnxStackConfig_IsValid(const CCNxStackConfig *instance)
{
    bool result = false;
    if (instance != NULL) {
        result = true;
    }
    return result;
}

PARCJSON *
ccnxStackConfig_ToJSON(const CCNxStackConfig *instance)
{
    ccnxStackConfig_OptionalAssertValid(instance);

    return instance->stackjson;
}

char *
ccnxStackConfig_ToString(const CCNxStackConfig *instance)
{
    PARCJSON *json = ccnxStackConfig_ToJSON(instance);

    char *result = parcJSON_ToString(json);

    return result;
}

PARCJSONValue *
ccnxStackConfig_Get(const CCNxStackConfig *config, const char *componentKey)
{
    ccnxStackConfig_OptionalAssertValid(config);
    PARCJSONValue *value = parcJSON_GetValueByName(config->stackjson, componentKey);
    return value;
}

PARCHashCode
ccnxStackConfig_HashCode(const CCNxStackConfig *config)
{
    ccnxStackConfig_OptionalAssertValid(config);
    return parcJSON_HashCode(config->stackjson);
}

CCNxStackConfig *
ccnxStackConfig_Add(CCNxStackConfig *config, const char *componentKey, PARCJSONValue *jsonObject)
{
    ccnxStackConfig_OptionalAssertValid(config);

    parcJSON_AddValue(config->stackjson, componentKey, jsonObject);
    return config;
}

PARCJSON *
ccnxStackConfig_GetJson(const CCNxStackConfig *config)
{
    ccnxStackConfig_OptionalAssertValid(config);

    return (config->stackjson);
}
