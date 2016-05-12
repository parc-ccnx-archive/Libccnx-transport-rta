/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
/*
 *
 */
#include "../cpi_ConnectionList.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>



LONGBOW_TEST_RUNNER(cpi_ConnectionList)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_ConnectionList)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_ConnectionList)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiConnectionList_Append);
    LONGBOW_RUN_TEST_CASE(Global, cpiConnectionList_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, cpiConnectionList_FromJson);
    LONGBOW_RUN_TEST_CASE(Global, cpiConnectionList_Equals);
    LONGBOW_RUN_TEST_CASE(Global, cpiConnectionList_ToJson);
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

static CPIConnection *
createConnectionObject(unsigned ifidx, int s_addr, uint16_t s_port, int d_addr, uint16_t d_port)
{
    return cpiConnection_Create(ifidx,
                                cpiAddress_CreateFromInet(&(struct sockaddr_in) { .sin_family = PF_INET, .sin_addr.s_addr = s_addr, .sin_port = s_port }),
                                cpiAddress_CreateFromInet(&(struct sockaddr_in) { .sin_family = PF_INET, .sin_addr.s_addr = d_addr, .sin_port = d_port }),
                                cpiConnection_TCP);
}

LONGBOW_TEST_CASE(Global, cpiConnectionList_Append)
{
    CPIConnectionList *list = cpiConnectionList_Create();
    cpiConnectionList_Append(list, createConnectionObject(1, 2, 3, 4, 5));

    assertTrue(parcArrayList_Size(list->listOfConnections) == 1, "got wrong size, expected %u got %zu", 1, parcArrayList_Size(list->listOfConnections));

    cpiConnectionList_Destroy(&list);
}

LONGBOW_TEST_CASE(Global, cpiConnectionList_Create_Destroy)
{
    CPIConnectionList *list = cpiConnectionList_Create();
    cpiConnectionList_Destroy(&list);
    assertTrue(parcMemory_Outstanding() == 0, "Memory imbalance after create/destroy");
}

LONGBOW_TEST_CASE(Global, cpiConnectionList_FromJson)
{
    // The JSON representation depends on the system sockaddr_in format, which
    // varies platform to platform.
#if defined(__APPLE__)
    char truth_string[] = "{\"ConnectionList\":[{\"Connection\":{\"IFIDX\":1,\"CONNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIDAAIAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIFAAQAAAAAAAAAAAAAAA==\"}}}]}";
#elif defined(__linux__)
    char truth_string[] = "{\"ConnectionList\":[{\"Connection\":{\"IFIDX\":1,\"CONNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgADAAIAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgAFAAQAAAAAAAAAAAAAAA==\"}}}]}";
#else
    // Case 1033
    testUnimplemented("Platform not supported");
    return;
#endif

    CPIConnectionList *truth_list = cpiConnectionList_Create();
    cpiConnectionList_Append(truth_list, createConnectionObject(1, 2, 3, 4, 5));

    PARCJSON *truth_json = parcJSON_ParseString(truth_string);
    CPIConnectionList *test_list = cpiConnectionList_FromJson(truth_json);

    assertTrue(cpiConnectionList_Equals(truth_list, test_list), "Lists do not match");

    cpiConnectionList_Destroy(&test_list);
    parcJSON_Release(&truth_json);
    cpiConnectionList_Destroy(&truth_list);
}

LONGBOW_TEST_CASE(Global, cpiConnectionList_Equals)
{
    CPIConnectionList *list_a = cpiConnectionList_Create();
    cpiConnectionList_Append(list_a, createConnectionObject(1, 2, 3, 4, 5));

    CPIConnectionList *list_b = cpiConnectionList_Create();
    cpiConnectionList_Append(list_b, createConnectionObject(1, 2, 3, 4, 5));

    CPIConnectionList *list_c = cpiConnectionList_Create();
    cpiConnectionList_Append(list_c, createConnectionObject(1, 2, 3, 4, 5));

    CPIConnectionList *unequal = cpiConnectionList_Create();
    cpiConnectionList_Append(unequal, createConnectionObject(99, 2, 3, 4, 5));
    cpiConnectionList_Append(unequal, createConnectionObject(1, 99, 3, 4, 5));
    cpiConnectionList_Append(unequal, createConnectionObject(1, 2, 99, 4, 5));
    cpiConnectionList_Append(unequal, createConnectionObject(1, 2, 3, 99, 5));
    cpiConnectionList_Append(unequal, createConnectionObject(1, 2, 3, 4, 99));

    assertEqualsContract(cpiConnectionList_Equals, list_a, list_b, list_c, unequal);

    cpiConnectionList_Destroy(&unequal);
    cpiConnectionList_Destroy(&list_a);
    cpiConnectionList_Destroy(&list_b);
    cpiConnectionList_Destroy(&list_c);
}

LONGBOW_TEST_CASE(Global, cpiConnectionList_ToJson)
{
    // The JSON representation depends on the system sockaddr_in format, which
    // varies platform to platform.
#if defined(__APPLE__)
    char truth_string[] = "{\"ConnectionList\":[{\"Connection\":{\"IFIDX\":1,\"CONNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIDAAIAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIFAAQAAAAAAAAAAAAAAA==\"}}}]}";
#elif defined(__linux__)
    char truth_string[] = "{\"ConnectionList\":[{\"Connection\":{\"IFIDX\":1,\"CONNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgADAAIAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgAFAAQAAAAAAAAAAAAAAA==\"}}}]}";
#else
    // Case 1033
    testUnimplemented("Platform not supported");
    return;
#endif


    CPIConnectionList *list = cpiConnectionList_Create();
    cpiConnectionList_Append(list, createConnectionObject(1, 2, 3, 4, 5));

    PARCJSON *json = cpiConnectionList_ToJson(list);
    char *test = parcJSON_ToCompactString(json);
    assertTrue(strcmp(truth_string, test) == 0, "Got wrong JSON.\nexpected: %s\ngot %s\n", truth_string, test);
    parcMemory_Deallocate((void **) &test);

    parcJSON_Release(&json);
    cpiConnectionList_Destroy(&list);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_ConnectionList);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
