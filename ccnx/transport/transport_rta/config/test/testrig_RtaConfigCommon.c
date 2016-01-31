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
 * Common test routines for the RTA component configuration functions
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

typedef struct test_data {
    CCNxConnectionConfig *connConfig;
    CCNxStackConfig *stackConfig;
} TestData;

TestData *
testRtaConfiguration_CommonSetup(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));
    data->connConfig = ccnxConnectionConfig_Create();
    data->stackConfig = ccnxStackConfig_Create();
    return data;
}

void
testRtaConfiguration_CommonTeardown(TestData *data)
{
    ccnxStackConfig_Release(&data->stackConfig);
    ccnxConnectionConfig_Destroy(&data->connConfig);
    parcMemory_Deallocate((void **) &data);
}

void
testRtaConfiguration_ComponentName(const char * (*getname)(void), const char *truth)
{
    const char *name = getname();
    assertTrue(strcmp(name, truth) == 0,
               "Got wrong name, got %s expected %s", name, truth);
}

void
testRtaConfiguration_ConnectionJsonKey(CCNxConnectionConfig *configToTest, const char *key)
{
    PARCJSON *json = ccnxConnectionConfig_GetJson(configToTest);
    PARCJSONValue *value = parcJSON_GetValueByName(json, key);
    assertNotNull(value, "Could not find key %s in configuration json", key);
}

void
testRtaConfiguration_ProtocolStackJsonKey(CCNxStackConfig *configToTest, const char *key)
{
    PARCJSON *json = ccnxStackConfig_GetJson(configToTest);
    PARCJSONValue *value = parcJSON_GetValueByName(json, key);
    assertNotNull(value, "Could not find key %s in configuration json", key);
}
