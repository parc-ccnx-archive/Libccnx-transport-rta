/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include "../rta_CommandCreateProtocolStack.c"

#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(rta_CommandCreateProtocolStack)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_CommandCreateProtocolStack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_CommandCreateProtocolStack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_Create);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid_NULL);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid_BadCCNxStackConfig);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_AssertValid);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetConfig);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetStackId);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetStackConfig);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCreateProtocolStack_Release);
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

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_Acquire)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    parcObjectTesting_AssertAcquire(createStack);

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_Create)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);
    assertNotNull(createStack, "Expected rtaCommandCreateProtocolStack_Create to return non-NULL.");

    assertTrue(createStack->stackId == stackId, "Expected stackId %d, actual %d", stackId, createStack->stackId);

    assertTrue(ccnxStackConfig_Equals(config, createStack->config),
               "ProtocolStackConfig instances are not equal");
    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    assertTrue(rtaCommandCreateProtocolStack_IsValid(createStack),
               "Expected rtaCommandCreateProtocolStack_Create to return a valid instance.");

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid_NULL)
{
    RtaCommandCreateProtocolStack *createStack = NULL;

    assertFalse(rtaCommandCreateProtocolStack_IsValid(createStack),
                "Expected rtaCommandCreateProtocolStack_Create to return a valid instance.");
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_IsValid_BadCCNxStackConfig)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);
    CCNxStackConfig *original = createStack->config;
    createStack->config = NULL; // Make it bad.
    assertFalse(rtaCommandCreateProtocolStack_IsValid(createStack),
               "Expected rtaCommandCreateProtocolStack_Create to return a valid instance.");
    createStack->config = original;
    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_AssertValid)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    rtaCommandCreateProtocolStack_AssertValid(createStack);

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetStackConfig)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    CCNxStackConfig *actual = rtaCommandCreateProtocolStack_GetStackConfig(createStack);

    assertTrue(ccnxStackConfig_Equals(config, actual),
               "CCNxStackConfig instances are not equal");

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetConfig)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    assertTrue(ccnxStackConfig_Equals(config, createStack->config),
               "ProtocolStackConfig instances are not equal");

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_GetStackId)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    int testStackId = rtaCommandCreateProtocolStack_GetStackId(createStack);
    assertTrue(testStackId == stackId, "Wrong value, got %d expected %d", testStackId, stackId);

    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommandCreateProtocolStack_Release)
{
    int stackId = 7;
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);

    RtaCommandCreateProtocolStack *second = rtaCommandCreateProtocolStack_Acquire(createStack);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    rtaCommandCreateProtocolStack_Release(&createStack);
    size_t thirdRefCount = parcObject_GetReferenceCount(second);

    assertTrue(thirdRefCount == secondRefCount - 1,
               "Wrong refcount after release, got %zu expected %zu", thirdRefCount, secondRefCount - 1);

    rtaCommandCreateProtocolStack_Release(&second);
    ccnxStackConfig_Release(&config);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_CommandCreateProtocolStack);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
