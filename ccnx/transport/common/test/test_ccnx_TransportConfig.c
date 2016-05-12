/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
// This permits internal static functions to be visible to this Test Runner.
#include "../ccnx_TransportConfig.c"

#include <LongBow/unit-test.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>
#include <parc/algol/parc_SafeMemory.h>


LONGBOW_TEST_RUNNER(ccnx_TransportConfig)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified here, but every test must be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Static);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(ccnx_TransportConfig)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(ccnx_TransportConfig)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_AssertValid);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_Copy);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_CreateDestroy);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_GetConnectionConfig);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_Equals);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_GetStackConfig);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_IsValid_True);
    LONGBOW_RUN_TEST_CASE(Global, ccnxTransportConfig_IsValid_False);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    parcSafeMemory_ReportAllocation(STDOUT_FILENO);

    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_AssertValid)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *x = ccnxTransportConfig_Create(stack, connection);

    ccnxTransportConfig_AssertValid(x);

    ccnxStackConfig_Release(&stack);

    ccnxTransportConfig_Destroy(&x);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_Copy)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *x = ccnxTransportConfig_Create(stack, connection);
    assertNotNull(x, "Expected non-null result from ccnxTransportConfig_Create");
    ccnxStackConfig_Release(&stack);

    CCNxTransportConfig *y = ccnxTransportConfig_Copy(x);

    assertTrue(ccnxTransportConfig_Equals(x, y), "Expected ccnxTransportConfig_Copy result to be equal to the original");

    ccnxTransportConfig_Destroy(&x);
    ccnxTransportConfig_Destroy(&y);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_Equals)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *x = ccnxTransportConfig_Create(stack, ccnxConnectionConfig_Copy(connection));
    CCNxTransportConfig *y = ccnxTransportConfig_Create(stack, ccnxConnectionConfig_Copy(connection));
    CCNxTransportConfig *z = ccnxTransportConfig_Create(stack, ccnxConnectionConfig_Copy(connection));

    CCNxStackConfig *otherStack = ccnxStackConfig_Create();
    PARCJSONValue *val = parcJSONValue_CreateFromNULL();
    ccnxStackConfig_Add(otherStack, "key", val);
    CCNxTransportConfig *u1 = ccnxTransportConfig_Create(otherStack, ccnxConnectionConfig_Copy(connection));

    CCNxConnectionConfig *otherConnection = ccnxConnectionConfig_Create();
    ccnxConnectionConfig_Add(otherConnection, "key", val);

    CCNxTransportConfig *u2 = ccnxTransportConfig_Create(stack, otherConnection);

    parcObjectTesting_AssertEqualsFunction(ccnxTransportConfig_Equals, x, y, z, u1, u2);

    assertTrue(ccnxTransportConfig_Equals(x, y), "Expected ccnxTransportConfig_Copy result to be equal to the original");

    parcJSONValue_Release(&val);
    ccnxStackConfig_Release(&stack);
    ccnxStackConfig_Release(&otherStack);
    ccnxConnectionConfig_Destroy(&connection);

    ccnxTransportConfig_Destroy(&x);
    ccnxTransportConfig_Destroy(&y);
    ccnxTransportConfig_Destroy(&z);
    ccnxTransportConfig_Destroy(&u1);
    ccnxTransportConfig_Destroy(&u2);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_CreateDestroy)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *x = ccnxTransportConfig_Create(stack, connection);
    assertNotNull(x, "Expected non-null result from ccnxTransportConfig_Create");
    ccnxStackConfig_Release(&stack);

    ccnxTransportConfig_Destroy(&x);
    assertNull(x, "Expected null result from ccnxStackConfig_Release");
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_GetConnectionConfig)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *config = ccnxTransportConfig_Create(stack, connection);
    ccnxStackConfig_Release(&stack);

    CCNxConnectionConfig *actual = ccnxTransportConfig_GetConnectionConfig(config);

    assertTrue(connection == actual, "Expected ccnxTransportConfig_GetConnectionConfig to return the original.");

    ccnxTransportConfig_Destroy(&config);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_GetStackConfig)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *config = ccnxTransportConfig_Create(stack, connection);

    CCNxStackConfig *actual = ccnxTransportConfig_GetStackConfig(config);

    assertTrue(stack == actual, "Expected ccnxTransportConfig_GetStackConfig to return the original.");

    ccnxStackConfig_Release(&stack);
    ccnxTransportConfig_Destroy(&config);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_IsValid_True)
{
    CCNxStackConfig *stack = ccnxStackConfig_Create();
    CCNxConnectionConfig *connection = ccnxConnectionConfig_Create();

    CCNxTransportConfig *x = ccnxTransportConfig_Create(stack, connection);
    assertTrue(ccnxTransportConfig_IsValid(x), "Expected ccnxTransportConfig_Create to return a valid instance.");
    ccnxStackConfig_Release(&stack);

    ccnxTransportConfig_Destroy(&x);
}

LONGBOW_TEST_CASE(Global, ccnxTransportConfig_IsValid_False)
{
    assertFalse(ccnxTransportConfig_IsValid(NULL), "Expected NULL to be an invalid instance.");
}

LONGBOW_TEST_FIXTURE(Static)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(ccnx_TransportConfig);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
