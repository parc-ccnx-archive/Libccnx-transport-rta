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
#include "../ccnx_StackConfig.c"
#include <LongBow/unit-test.h>

#include <inttypes.h>
#include <stdio.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(ccnx_StackConfig)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified here, but every test must be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Static);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(ccnx_StackConfig)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(ccnx_StackConfig)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_AddGet);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_AssertValid);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_Copy);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_CreateAcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_Display);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_Equals);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_HashCode);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_GetJson);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_IsValid);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_ToJSON);
    LONGBOW_RUN_TEST_CASE(Global, ccnxStackConfig_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_AddGet)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();

    PARCJSONValue *expected = parcJSONValue_CreateFromNULL();
    ccnxStackConfig_Add(instance, "key", expected);

    PARCJSONValue *actual = ccnxStackConfig_Get(instance, "key");

    assertTrue(parcJSONValue_Equals(expected, actual), "ccnxStackConfig_Get did not return what was 'added'");

    parcJSONValue_Release(&expected);

    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_AssertValid)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    ccnxStackConfig_AssertValid(instance);

    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_Copy)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    CCNxStackConfig *copy = ccnxStackConfig_Copy(instance);
    assertTrue(ccnxStackConfig_Equals(instance, copy), "Expected the copy to be equal to the original");

    ccnxStackConfig_Release(&instance);
    ccnxStackConfig_Release(&copy);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_CreateAcquireRelease)
{
    CCNxStackConfig *config = ccnxStackConfig_Create();
    assertNotNull(config, "Expected non-NULL result from ccnxConnectionConfig_Create.");

    CCNxStackConfig *reference = ccnxStackConfig_Acquire(config);

    ccnxStackConfig_Release(&config);
    assertNull(config, "Expected NULL result from ccnxConnectionConfig_Destroy");
    ccnxStackConfig_Release(&reference);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_Display)
{
    CCNxStackConfig *config = ccnxStackConfig_Create();
    ccnxStackConfig_Display(config, 1);

    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_Equals)
{
    CCNxStackConfig *x = ccnxStackConfig_Create();
    CCNxStackConfig *y = ccnxStackConfig_Create();
    CCNxStackConfig *z = ccnxStackConfig_Create();

    CCNxStackConfig *u1 = ccnxStackConfig_Create();
    PARCJSONValue *val = parcJSONValue_CreateFromNULL();
    ccnxStackConfig_Add(u1, "key", val);
    parcJSONValue_Release(&val);

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    ccnxStackConfig_Release(&x);
    ccnxStackConfig_Release(&y);
    ccnxStackConfig_Release(&z);
    ccnxStackConfig_Release(&u1);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_HashCode)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    uint64_t hashCode = ccnxStackConfig_HashCode(instance);
    printf("%" PRIu64 "\n", hashCode);
    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_GetJson)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    PARCJSON *json = ccnxStackConfig_GetJson(instance);

    assertNotNull(json, "Expected non-null JSON");
    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_IsValid)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    assertTrue(ccnxStackConfig_IsValid(instance), "Expected ccnxStackConfig_Create to result in a valid instance.");

    ccnxStackConfig_Release(&instance);
    assertFalse(ccnxStackConfig_IsValid(instance), "Expected ccnxStackConfig_Create to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_ToJSON)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    PARCJSON *json = ccnxStackConfig_ToJSON(instance);
    assertNotNull(json, "Expected non-null JSON");

    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_ToString)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();
    char *string = ccnxStackConfig_ToString(instance);
    assertNotNull(string, "Expected non-null ccnxStackConfig_ToString");

    parcMemory_Deallocate((void **) &string);
    ccnxStackConfig_Release(&instance);
}

LONGBOW_TEST_CASE(Global, ccnxStackConfig_Get)
{
    CCNxStackConfig *instance = ccnxStackConfig_Create();

    ccnxStackConfig_Release(&instance);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(ccnx_StackConfig);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
