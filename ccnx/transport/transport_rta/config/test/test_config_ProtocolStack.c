/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../config_ProtocolStack.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include "testrig_RtaConfigCommon.c"

#include <ccnx/transport/common/ccnx_TransportConfig.h>

LONGBOW_TEST_RUNNER(config_ProtocolStack)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(config_ProtocolStack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(config_ProtocolStack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, protocolStack_ComponentsConfigArgs);
    LONGBOW_RUN_TEST_CASE(Global, protocolStack_ComponentsConfigArrayList);
    LONGBOW_RUN_TEST_CASE(Global, protocolStack_GetComponentNameArray);
    LONGBOW_RUN_TEST_CASE(Global, protocolStack_GetName);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, protocolStack_ComponentsConfigArgs)
{
    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();

    const char truth[] = "{\"STACK\":{\"COMPONENTS\":[\"Apple\",\"Bananna\",\"Cherry\"]}}";

    protocolStack_ComponentsConfigArgs(stackConfig, "Apple", "Bananna", "Cherry", NULL);
    PARCJSON *json = ccnxStackConfig_GetJson(stackConfig);
    char *str = parcJSON_ToCompactString(json);
    assertTrue(strcmp(truth, str) == 0, "Got wrong config, got %s expected %s", str, truth);
    parcMemory_Deallocate((void **) &str);
    ccnxStackConfig_Release(&stackConfig);
}

LONGBOW_TEST_CASE(Global, protocolStack_ComponentsConfigArrayList)
{
    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();
    PARCArrayList *names = parcArrayList_Create(NULL);
    parcArrayList_Add(names, "Apple");
    parcArrayList_Add(names, "Bananna");
    parcArrayList_Add(names, "Cherry");

    const char truth[] = "{\"STACK\":{\"COMPONENTS\":[\"Apple\",\"Bananna\",\"Cherry\"]}}";

    protocolStack_ComponentsConfigArrayList(stackConfig, names);
    PARCJSON *json = ccnxStackConfig_GetJson(stackConfig);
    char *str = parcJSON_ToCompactString(json);
    assertTrue(strcmp(truth, str) == 0, "Got wrong config, got %s expected %s", str, truth);

    parcMemory_Deallocate((void **) &str);
    ccnxStackConfig_Release(&stackConfig);
    parcArrayList_Destroy(&names);
}

LONGBOW_TEST_CASE(Global, protocolStack_GetComponentNameArray)
{
    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();
    PARCArrayList *names = parcArrayList_Create(NULL);
    parcArrayList_Add(names, "Apple");
    parcArrayList_Add(names, "Bananna");
    parcArrayList_Add(names, "Cherry");

    protocolStack_ComponentsConfigArrayList(stackConfig, names);

    char truth[] = "{\"STACK\":{\"COMPONENTS\":[\"Apple\",\"Bananna\",\"Cherry\"]}}";
    PARCJSON *json = parcJSON_ParseString(truth);

    PARCArrayList *test = protocolStack_GetComponentNameArray(json);

    assertTrue(parcArrayList_Size(test) == parcArrayList_Size(names),
               "wrong array list size, got %zu expected %zu",
               parcArrayList_Size(test), parcArrayList_Size(names));
    for (int i = 0; i < parcArrayList_Size(test); i++) {
        char *a = parcArrayList_Get(test, i);
        char *b = parcArrayList_Get(names, i);
        assertTrue(strcmp(a, b) == 0, "mismatch elements %d, got %s expected %s", i, a, b);
    }

    ccnxStackConfig_Release(&stackConfig);
    parcArrayList_Destroy(&names);
    parcJSON_Release(&json);
    parcArrayList_Destroy(&test);
}

LONGBOW_TEST_CASE(Global, protocolStack_GetName)
{
    const char *name = protocolStack_GetName();
    assertTrue(strcmp(name, param_STACK) == 0, "Got wrong name, got %s expected %s", name, param_STACK);
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(config_ProtocolStack);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
