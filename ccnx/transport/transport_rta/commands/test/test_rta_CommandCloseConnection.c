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
#include "../rta_CommandCloseConnection.c"

#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(rta_CommandCloseConnection)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_CommandCloseConnection)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_CommandCloseConnection)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCloseConnection_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCloseConnection_Create);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCloseConnection_GetApiNotifierFd);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommandCloseConnection_Release);
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

LONGBOW_TEST_CASE(Global, rtaCommandCloseConnection_Acquire)
{
    int apiNotifierFd = 7;
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(apiNotifierFd);
    size_t firstRefCount = parcObject_GetReferenceCount(closeConnection);

    RtaCommandCloseConnection *second = rtaCommandCloseConnection_Acquire(closeConnection);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    assertTrue(secondRefCount == firstRefCount + 1, "Wrong refcount after acquire, got %zu expected %zu", secondRefCount, firstRefCount + 1);

    rtaCommandCloseConnection_Release(&closeConnection);
    rtaCommandCloseConnection_Release(&second);
}

LONGBOW_TEST_CASE(Global, rtaCommandCloseConnection_Create)
{
    int apiNotifierFd = 7;
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(apiNotifierFd);
    assertNotNull(closeConnection, "Got null from create");
    assertTrue(closeConnection->apiNotifierFd == apiNotifierFd, "Internal apiSocket wrong, got %d expected %d", closeConnection->apiNotifierFd, apiNotifierFd);
    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommandCloseConnection_GetApiNotifierFd)
{
    int apiNotifierFd = 7;
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(apiNotifierFd);

    int testFd = rtaCommandCloseConnection_GetApiNotifierFd(closeConnection);
    assertTrue(testFd == apiNotifierFd, "Wrong value, got %d expected %d", testFd, apiNotifierFd);

    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommandCloseConnection_Release)
{
    int apiNotifierFd = 7;
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(apiNotifierFd);

    RtaCommandCloseConnection *second = rtaCommandCloseConnection_Acquire(closeConnection);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    rtaCommandCloseConnection_Release(&closeConnection);
    size_t thirdRefCount = parcObject_GetReferenceCount(second);

    assertTrue(thirdRefCount == secondRefCount - 1, "Wrong refcount after release, got %zu expected %zu", thirdRefCount, secondRefCount - 1);

    rtaCommandCloseConnection_Release(&second);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_CommandCloseConnection);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
