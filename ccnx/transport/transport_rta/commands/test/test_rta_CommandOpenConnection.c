/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../rta_CommandOpenConnection.c"

#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

typedef struct test_data {
    int stackId;
    int apiNotifierFd;
    int transportNotifierFd;
    PARCJSON *config;

    RtaCommandOpenConnection *openConnection;
} TestData;

static TestData *
_createTestData(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));

    data->stackId = 7;
    data->apiNotifierFd = 11;
    data->transportNotifierFd = 10029;
    data->config = parcJSON_Create();

    data->openConnection = rtaCommandOpenConnection_Create(data->stackId, data->apiNotifierFd, data->transportNotifierFd, data->config);

    return data;
}

static void
_destroyTestData(TestData **dataPtr)
{
    TestData *data = *dataPtr;

    rtaCommandOpenConnection_Release(&data->openConnection);
    parcJSON_Release(&data->config);
    parcMemory_Deallocate((void **) &data);

    *dataPtr = NULL;
}

// =============================================================

LONGBOW_TEST_RUNNER(rta_CommandOpenConnection)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_CommandOpenConnection)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_CommandOpenConnection)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_Create);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_GetApiNotifierFd);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_GetConfig);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_GetStackId);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_GetTransportNotifierFd);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandOpenConnection_Release);
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

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_Acquire)
{
    TestData *data = _createTestData();

    size_t firstRefCount = parcObject_GetReferenceCount(data->openConnection);

    RtaCommandOpenConnection *second = rtaCommandOpenConnection_Acquire(data->openConnection);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    assertTrue(secondRefCount == firstRefCount + 1, "Wrong refcount after acquire, got %zu expected %zu", secondRefCount, firstRefCount + 1);

    rtaCommandOpenConnection_Release(&second);
    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_Create)
{
    TestData *data = _createTestData();
    assertNotNull(data->openConnection, "Got null from create");
    assertTrue(data->openConnection->stackId == data->stackId, "Internal stackId wrong, got %d expected %d",
               data->openConnection->stackId, data->stackId);
    assertTrue(data->openConnection->apiNotifierFd == data->apiNotifierFd, "Internal apiNotifierFd wrong, got %d expected %d",
               data->openConnection->apiNotifierFd, data->apiNotifierFd);
    assertTrue(data->openConnection->transportNotifierFd == data->transportNotifierFd, "Internal transportNotifierFd wrong, got %d expected %d",
               data->openConnection->transportNotifierFd, data->transportNotifierFd);
    assertTrue(parcJSON_Equals(data->openConnection->config, data->config), "JSON configs are not equal");
    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_GetApiNotifierFd)
{
    TestData *data = _createTestData();

    int testApiFd = rtaCommandOpenConnection_GetApiNotifierFd(data->openConnection);
    assertTrue(testApiFd == data->apiNotifierFd, "Wrong value, got %d expected %d", testApiFd, data->apiNotifierFd);

    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_GetConfig)
{
    TestData *data = _createTestData();

    PARCJSON *testConfig = rtaCommandOpenConnection_GetConfig(data->openConnection);
    assertTrue(parcJSON_Equals(data->config, testConfig), "Configurations do not match");

    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_GetStackId)
{
    TestData *data = _createTestData();

    int testStackId = rtaCommandOpenConnection_GetStackId(data->openConnection);
    assertTrue(testStackId == data->stackId, "Wrong value, got %d expected %d", testStackId, data->stackId);

    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_GetTransportNotifierFd)
{
    TestData *data = _createTestData();

    int testTransportFd = rtaCommandOpenConnection_GetTransportNotifierFd(data->openConnection);
    assertTrue(testTransportFd == data->transportNotifierFd, "Wrong value, got %d expected %d", testTransportFd, data->transportNotifierFd);

    _destroyTestData(&data);
}

LONGBOW_TEST_CASE(Global, rtaCommandOpenConnection_Release)
{
    TestData *data = _createTestData();

    RtaCommandOpenConnection *second = rtaCommandOpenConnection_Acquire(data->openConnection);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    rtaCommandOpenConnection_Release(&second);
    size_t thirdRefCount = parcObject_GetReferenceCount(data->openConnection);

    assertTrue(thirdRefCount == secondRefCount - 1, "Wrong refcount after release, got %zu expected %zu", thirdRefCount, secondRefCount - 1);

    _destroyTestData(&data);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_CommandOpenConnection);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
