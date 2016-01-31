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
#include "../config_Signer.c"
#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include "testrig_RtaConfigCommon.c"

LONGBOW_TEST_RUNNER(config_Signer)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(config_Signer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(config_Signer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, signer_GetImplementationType_PublicKey);
    LONGBOW_RUN_TEST_CASE(Global, signer_GetImplementationType_SymmetricKey);
    LONGBOW_RUN_TEST_CASE(Global, signer_GetImplementationType_Unknown);
    LONGBOW_RUN_TEST_CASE(Global, signer_GetName);
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

LONGBOW_TEST_CASE(Global, signer_GetImplementationType_PublicKey)
{
    CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
    const char *filename = "filename";
    const char *password = "password";
    publicKeySignerPkcs12Store_ConnectionConfig(connConfig, filename, password);

    PARCJSON *json = ccnxConnectionConfig_GetJson(connConfig);

    SignerType type = signer_GetImplementationType(json);
    assertTrue(type == SIGNER_PublicKeySignerPkcs12Store, "Got wrong signer type, got %d expected %d", type, SIGNER_PublicKeySignerPkcs12Store);

    ccnxConnectionConfig_Destroy(&connConfig);
}

LONGBOW_TEST_CASE(Global, signer_GetImplementationType_SymmetricKey)
{
    CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
    const char *filename = "filename";
    const char *password = "password";
    symmetricKeySignerFileStore_ConnectionConfig(connConfig, filename, password);

    PARCJSON *json = ccnxConnectionConfig_GetJson(connConfig);

    SignerType type = signer_GetImplementationType(json);
    assertTrue(type == SIGNER_SymmetricKeySignerFileStore, "Got wrong signer type, got %d expected %d", type, SIGNER_SymmetricKeySignerFileStore);

    ccnxConnectionConfig_Destroy(&connConfig);
}

LONGBOW_TEST_CASE(Global, signer_GetImplementationType_Unknown)
{
    char *bogusSignerString = "{\"SIGNER\":\"BogusSigner\",\"BogusSigner\":{}}";

    PARCJSON *json = parcJSON_ParseString(bogusSignerString);

    SignerType type = signer_GetImplementationType(json);
    assertTrue(type == SIGNER_Unknown, "Got wrong signer type, got %d expected %d", type, SIGNER_Unknown);

    parcJSON_Release(&json);
}

LONGBOW_TEST_CASE(Global, signer_GetName)
{
    testRtaConfiguration_ComponentName(&signer_GetName, param_SIGNER);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(config_Signer);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
