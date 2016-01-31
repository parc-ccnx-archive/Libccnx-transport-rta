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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../cpi_ControlFacade.c"

#include <parc/algol/parc_SafeMemory.h>

#include <ccnx/transport/common/transport_MetaMessage.h>

#include <LongBow/unit-test.h>

typedef struct test_data {
    char *jsonstring;
    PARCJSON *json;
} TestData;

static TestData *
_commonSetup(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));
    data->jsonstring = parcMemory_StringDuplicate("{ \"EMPTY\": \"NESS\" }", 100);
    data->json = parcJSON_ParseString(data->jsonstring);
    assertNotNull(data->json, "got null JSON from string: %s", data->jsonstring);

    return data;
}

static void
_commonTeardown(TestData *data)
{
    parcJSON_Release(&data->json);
    parcMemory_Deallocate((void **) &(data->jsonstring));
    parcMemory_Deallocate((void **) &data);
}

LONGBOW_TEST_RUNNER(ccnx_ControlFacade)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(ccnx_ControlFacade)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(ccnx_ControlFacade)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_CreateControlMessage_Notification);

    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_AssertValid);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_CreateCPI);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_CreateNotification);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_GetJson);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_IsCPI);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_IsNotification);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_Display);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControlFacade_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    longBowTestCase_SetClipBoardData(testCase, _commonSetup());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    _commonTeardown(longBowTestCase_GetClipBoardData(testCase));
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_CreateControlMessage_Notification)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxMetaMessage *control = ccnxControlFacade_CreateNotification(data->json);

    CCNxControl *cpiControl = ccnxMetaMessage_GetControl(control);

    assertNotNull(cpiControl, "Got null control message");

    ccnxMetaMessage_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_AssertValid)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateCPI(data->json);
    ccnxControlFacade_AssertValid(control);
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_CreateCPI)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateCPI(data->json);
    ccnxControlFacade_AssertValid(control);
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_CreateNotification)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateNotification(data->json);
    ccnxControlFacade_AssertValid(control);
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_GetJson)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateNotification(data->json);

    PARCJSON *json = ccnxControlFacade_GetJson(control);

    char *test = parcJSON_ToCompactString(json);
    char *truth = parcJSON_ToCompactString(data->json);

    assertTrue(strcmp(test, truth) == 0, "Wrong JSON\ngot %s\nexpected %s\n", test, truth);

    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth);
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_IsCPI)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateCPI(data->json);
    assertTrue(ccnxControlFacade_IsCPI(control), "Notification says its not a notification");
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_IsNotification)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateNotification(data->json);
    assertTrue(ccnxControlFacade_IsNotification(control), "Notification says its not a notification");
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_Display)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateNotification(data->json);
    ccnxControlFacade_Display(control, 1);
    ccnxTlvDictionary_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControlFacade_ToString)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    CCNxTlvDictionary *control = ccnxControlFacade_CreateNotification(data->json);
    char *desc = ccnxControlFacade_ToString(control);

    assertNotNull(desc, "Expected a string");
    printf("%s\n", desc);

    parcMemory_Deallocate((void **) &desc);
    ccnxTlvDictionary_Release(&control);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(ccnx_ControlFacade);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
