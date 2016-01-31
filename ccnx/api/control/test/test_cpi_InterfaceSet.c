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

#include "../cpi_InterfaceSet.c"
#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>



LONGBOW_TEST_RUNNER(cpi_InterfaceSet)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_InterfaceSet)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_InterfaceSet)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_Add_Single);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_Add_TwoUnique);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_Add_TwoSame);

    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_FromJson);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_GetByInterfaceIndex);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_GetByName);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_GetByOrdinalIndex);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_Length);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterfaceSet_ToJson);
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

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_Add_Single)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface = cpiInterface_Create("eth0", 11, false, true, 1500);
    bool success = cpiInterfaceSet_Add(set, iface);
    assertTrue(success, "Adding one interface did not succeed");
    assertTrue(parcArrayList_Size(set->listOfInterfaces) == 1, "List wrong size, expected %u got %zu", 1, parcArrayList_Size(set->listOfInterfaces));
    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_Add_TwoUnique)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    bool success = cpiInterfaceSet_Add(set, iface0);
    assertTrue(success, "Adding one interface did not succeed");

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    success = cpiInterfaceSet_Add(set, iface1);
    assertTrue(success, "Adding second interface did not succeed");

    assertTrue(parcArrayList_Size(set->listOfInterfaces) == 2, "List wrong size, expected %u got %zu", 2, parcArrayList_Size(set->listOfInterfaces));
    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_Add_TwoSame)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    bool success = cpiInterfaceSet_Add(set, iface0);
    assertTrue(success, "Adding one interface did not succeed");

    CPIInterface *iface1 = cpiInterface_Create("eth0", 11, false, true, 1500);
    success = cpiInterfaceSet_Add(set, iface1);
    assertFalse(success, "Adding second interface duplicate interface should have failed");
    cpiInterface_Destroy(&iface1);

    assertTrue(parcArrayList_Size(set->listOfInterfaces) == 1, "List wrong size, expected %u got %zu", 1, parcArrayList_Size(set->listOfInterfaces));
    cpiInterfaceSet_Destroy(&set);
}


LONGBOW_TEST_CASE(Global, cpiInterfaceSet_Create_Destroy)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();
    cpiInterfaceSet_Destroy(&set);
    assertTrue(parcMemory_Outstanding() == 0, "Memory imbalance after create/destroy");
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_GetByInterfaceIndex)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    CPIInterface *test = cpiInterfaceSet_GetByInterfaceIndex(set, 11);

    assertTrue(cpiInterface_Equals(test, iface0), "Did not get back right interface");

    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_GetByName)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    CPIInterface *test = cpiInterfaceSet_GetByName(set, "eth0");

    assertTrue(cpiInterface_Equals(test, iface0), "Did not get back right interface");

    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_GetByOrdinalIndex)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    CPIInterface *test = cpiInterfaceSet_GetByOrdinalIndex(set, 0);

    assertTrue(cpiInterface_Equals(test, iface0), "Did not get back right interface");

    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_Length)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    size_t length = cpiInterfaceSet_Length(set);

    assertTrue(length == 2, "Wrong length, expected %u got %zu", 2, length);

    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_ToJson)
{
    char truth[] = "{\"Interfaces\":[{\"Interface\":{\"Name\":\"eth0\",\"Index\":11,\"Loopback\":\"false\",\"Multicast\":\"true\",\"MTU\":1500,\"Addrs\":[]}},{\"Interface\":{\"Name\":\"eth1\",\"Index\":12,\"Loopback\":\"false\",\"Multicast\":\"true\",\"MTU\":1500,\"Addrs\":[]}}]}";

    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    PARCJSON *json = cpiInterfaceSet_ToJson(set);
    char *str = parcJSON_ToCompactString(json);
    assertTrue(strcasecmp(truth, str) == 0, "Json wrong, expected '%s' got '%s'", truth, str);
    parcMemory_Deallocate((void **) &str);

    parcJSON_Release(&json);
    cpiInterfaceSet_Destroy(&set);
}

LONGBOW_TEST_CASE(Global, cpiInterfaceSet_FromJson)
{
    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    CPIInterface *iface0 = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(set, iface0);

    CPIInterface *iface1 = cpiInterface_Create("eth1", 12, false, true, 1500);
    cpiInterfaceSet_Add(set, iface1);

    PARCJSON *json = cpiInterfaceSet_ToJson(set);

    CPIInterfaceSet *test_set = cpiInterfaceSet_FromJson(json);

    assertTrue(cpiInterfaceSet_Equals(set, test_set), "CPIInterfaceSet from json did not equal truth set");

    parcJSON_Release(&json);
    cpiInterfaceSet_Destroy(&set);
    cpiInterfaceSet_Destroy(&test_set);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_InterfaceSet);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
