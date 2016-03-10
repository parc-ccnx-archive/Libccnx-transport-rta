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

#include "../cpi_CancelFlow.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>

#include <inttypes.h>



LONGBOW_TEST_RUNNER(cpi_CancelFlow)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_CancelFlow)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_CancelFlow)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiCancelFlow_CreateRequest);
    LONGBOW_RUN_TEST_CASE(Global, cpiCancelFlow_NameFromControlMessage);
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

LONGBOW_TEST_CASE(Global, cpiCancelFlow_CreateRequest)
{
    const char truth_format[] = "{\"CPI_REQUEST\":{\"SEQUENCE\":%" PRIu64 ",\"CPI_CANCEL_FLOW\":{\"FLOW_NAME\":\"ccnx:/who/doesnt/like/pie\"}}}";

    CCNxName *name = ccnxName_CreateFromCString("lci:/who/doesnt/like/pie");
    PARCJSON *cpiRequest = cpiCancelFlow_CreateRequest(name);
    CCNxControl *controlRequest = ccnxControl_CreateCPIRequest(cpiRequest);

    PARCJSON *json = ccnxControl_GetJson(controlRequest);

    char buffer[1024];
    sprintf(buffer, truth_format, cpi_GetSequenceNumber(controlRequest));

    char *test_string = parcJSON_ToCompactString(json);
    assertTrue(strcmp(buffer, test_string) == 0, "Incorrect JSON, expected '%s' got '%s'", buffer, test_string);
    parcMemory_Deallocate((void **) &test_string);

    ccnxControl_Release(&controlRequest);
    parcJSON_Release(&cpiRequest);
    ccnxName_Release(&name);
}

LONGBOW_TEST_CASE(Global, cpiCancelFlow_NameFromControlMessage)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/who/doesnt/like/pie");
    PARCJSON *cpiRequest = cpiCancelFlow_CreateRequest(name);
    CCNxControl *controlRequest = ccnxControl_CreateCPIRequest(cpiRequest);

    CCNxName *test_name = cpiCancelFlow_NameFromControlMessage(controlRequest);
    assertTrue(ccnxName_Equals(test_name, name),
               "Expected %s actual %s",
               ccnxName_ToString(name),
               ccnxName_ToString(test_name));

    ccnxName_Release(&test_name);
    ccnxControl_Release(&controlRequest);
    parcJSON_Release(&cpiRequest);
    ccnxName_Release(&name);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_CancelFlow);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
