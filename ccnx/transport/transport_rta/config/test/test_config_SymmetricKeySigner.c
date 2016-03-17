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
#include "../config_SymmetricKeySigner.c"
#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include "testrig_RtaConfigCommon.c"
#include <ccnx/transport/transport_rta/config/config_Signer.h>

LONGBOW_TEST_RUNNER(config_SymmetricKeySignerFileStore)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(config_SymmetricKeySignerFileStore)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(config_SymmetricKeySignerFileStore)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, symmetricKeySignerFileStore_ConnectionConfig);
    LONGBOW_RUN_TEST_CASE(Global, symmetricKeySignerFileStore_GetConnectionParams);
    LONGBOW_RUN_TEST_CASE(Global, symmetricKeySignerFileStore_GetName);
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

LONGBOW_TEST_CASE(Global, symmetricKeySignerFileStore_ConnectionConfig)
{
    CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
    const char *filename = "filename";
    const char *password = "password";
    symmetricKeySigner_ConnectionConfig(connConfig, filename, password);

    // make sure our stuff is in there
    testRtaConfiguration_ConnectionJsonKey(connConfig, symmetricKeySigner_GetName());

    // make sure the SIGNER parameter is in there
    testRtaConfiguration_ConnectionJsonKey(connConfig, signer_GetName());

    ccnxConnectionConfig_Destroy(&connConfig);
}

LONGBOW_TEST_CASE(Global, symmetricKeySignerFileStore_GetConnectionParams)
{
    CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
    const char *filename = "filename";
    const char *password = "password";
    symmetricKeySigner_ConnectionConfig(connConfig, filename, password);

    PARCJSON *json = ccnxConnectionConfig_GetJson(connConfig);
    struct symmetrickeysigner_params params;

    symmetricKeySigner_GetConnectionParams(json, &params);

    assertTrue(strncmp(params.filename, filename, strlen(filename)) == 0, "wrong filename, got %s expected %s", params.filename, filename);
    assertTrue(strncmp(params.password, password, strlen(password)) == 0, "wrong password, got %s expected %s", params.password, password);

    ccnxConnectionConfig_Destroy(&connConfig);
}

LONGBOW_TEST_CASE(Global, symmetricKeySignerFileStore_GetName)
{
    testRtaConfiguration_ComponentName(&symmetricKeySigner_GetName, name);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(config_SymmetricKeySignerFileStore);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
