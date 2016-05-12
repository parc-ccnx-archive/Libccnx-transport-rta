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
#include "../cpi_AddressList.c"
#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>


LONGBOW_TEST_RUNNER(cpi_AddressList)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_AddressList)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_AddressList)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Append);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Copy);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_GetItem);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Equals_same_pointer);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Equals_both_empty);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Equals_unequal_sizes);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Equals_same_lists);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_Equals_wrong_order);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_ToJSON);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_FromJSON);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_ToFromJSON);
    LONGBOW_RUN_TEST_CASE(Global, cpiAddressList_ToString);
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

LONGBOW_TEST_CASE(Global, cpiAddressList_Append)
{
    CPIAddressList *list = cpiAddressList_Create();
    unsigned loops = 10;

    for (unsigned i = 0; i < loops; i++) {
        cpiAddressList_Append(list, cpiAddress_CreateFromInterface(i));
    }

    assertTrue(cpiAddressList_Length(list) == loops,
               "Got wrong length, expected %u got %zu",
               loops,
               cpiAddressList_Length(list));

    cpiAddressList_Destroy(&list);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Copy)
{
    CPIAddressList *list = cpiAddressList_Create();
    unsigned loops = 10;

    for (unsigned i = 0; i < loops; i++) {
        cpiAddressList_Append(list, cpiAddress_CreateFromInterface(i));
    }

    CPIAddressList *copy = cpiAddressList_Copy(list);
    assertTrue(cpiAddressList_Length(copy) == cpiAddressList_Length(list),
               "Copy wrong size, got %zu expected %zu",
               cpiAddressList_Length(copy),
               cpiAddressList_Length(list));

    for (unsigned i = 0; i < cpiAddressList_Length(copy); i++) {
        const CPIAddress *truth = cpiAddressList_GetItem(list, i);
        const CPIAddress *test = cpiAddressList_GetItem(copy, i);
        assertTrue(cpiAddress_Equals(truth, test), "Lists do not match at element %u", i);
    }

    cpiAddressList_Destroy(&list);
    cpiAddressList_Destroy(&copy);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Create_Destroy)
{
    CPIAddressList *list = cpiAddressList_Create();
    cpiAddressList_Destroy(&list);
    assertTrue(parcMemory_Outstanding() == 0, "Got memory imbalance: %u", parcMemory_Outstanding());
}

LONGBOW_TEST_CASE(Global, cpiAddressList_GetItem)
{
    CPIAddressList *list = cpiAddressList_Create();
    unsigned loops = 10;

    for (unsigned i = 0; i < loops; i++) {
        cpiAddressList_Append(list, cpiAddress_CreateFromInterface(i));
    }

    assertTrue(cpiAddressList_Length(list) == loops,
               "Got wrong length, expected %u got %zu",
               loops,
               cpiAddressList_Length(list));

    CPIAddress *truth = cpiAddress_CreateFromInterface(5);
    const CPIAddress *test = cpiAddressList_GetItem(list, 5);
    assertTrue(cpiAddress_Equals(truth, test), "Item 5 did not match!");

    cpiAddressList_Destroy(&list);
    cpiAddress_Destroy(&truth);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Equals_same_pointer)
{
    CPIAddressList *list = cpiAddressList_Create();
    assertTrue(cpiAddressList_Equals(list, list), "list != list, that's wrong");
    cpiAddressList_Destroy(&list);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Equals_both_empty)
{
    CPIAddressList *a = cpiAddressList_Create();
    CPIAddressList *b = cpiAddressList_Create();
    assertTrue(cpiAddressList_Equals(a, b), "emtpy list != empty list, that's wrong");
    cpiAddressList_Destroy(&a);
    cpiAddressList_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Equals_unequal_sizes)
{
    CPIAddressList *a = cpiAddressList_Create();
    CPIAddressList *b = cpiAddressList_Create();
    cpiAddressList_Append(a, cpiAddress_CreateFromInterface(1));
    assertFalse(cpiAddressList_Equals(a, b), "length 0 == length 1, that's wrong");
    cpiAddressList_Destroy(&a);
    cpiAddressList_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Equals_same_lists)
{
    CPIAddressList *a = cpiAddressList_Create();
    CPIAddressList *b = cpiAddressList_Create();
    cpiAddressList_Append(a, cpiAddress_CreateFromInterface(1));
    cpiAddressList_Append(a, cpiAddress_CreateFromInterface(2));
    cpiAddressList_Append(b, cpiAddress_CreateFromInterface(1));
    cpiAddressList_Append(b, cpiAddress_CreateFromInterface(2));
    assertTrue(cpiAddressList_Equals(a, b), "same lists not equal, that's wrong");
    cpiAddressList_Destroy(&a);
    cpiAddressList_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_Equals_wrong_order)
{
    CPIAddressList *a = cpiAddressList_Create();
    CPIAddressList *b = cpiAddressList_Create();
    cpiAddressList_Append(a, cpiAddress_CreateFromInterface(1));
    cpiAddressList_Append(a, cpiAddress_CreateFromInterface(2));
    cpiAddressList_Append(b, cpiAddress_CreateFromInterface(2));
    cpiAddressList_Append(b, cpiAddress_CreateFromInterface(1));
    assertFalse(cpiAddressList_Equals(a, b), "out of order lists equal, that's wrong");
    cpiAddressList_Destroy(&a);
    cpiAddressList_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_ToJSON)
{
    char truth[] = "[{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAA==\"},{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAQ==\"}]";

    CPIAddressList *a = cpiAddressList_Create();
    int loops = 2;
    for (int i = 0; i < loops; i++) {
        cpiAddressList_Append(a, cpiAddress_CreateFromInterface(i));
    }

    PARCJSONArray *jsonArray = cpiAddressList_ToJson(a);
    char *test = parcJSONArray_ToCompactString(jsonArray);

    assertTrue(strcmp(truth, test) == 0, "JSON strings did not match, got '%s' expected '%s'", test, truth);

    cpiAddressList_Destroy(&a);
    parcMemory_Deallocate((void **) &test);
    parcJSONArray_Release(&jsonArray);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_FromJSON)
{
    char json_str[] = "{\"ARRAY\":[{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAA==\"},{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAQ==\"}]}";
    PARCJSON *json = parcJSON_ParseString(json_str);

    PARCJSONArray *jsonArray = parcJSONValue_GetArray(parcJSON_GetValueByIndex(json, 0));

    CPIAddressList *test_list = cpiAddressList_CreateFromJson(jsonArray);

    CPIAddressList *truth_list = cpiAddressList_Create();
    int loops = 2;
    for (int i = 0; i < loops; i++) {
        cpiAddressList_Append(truth_list, cpiAddress_CreateFromInterface(i));
    }

    assertTrue(cpiAddressList_Equals(truth_list, test_list), "Lists did not match!");

    cpiAddressList_Destroy(&truth_list);
    cpiAddressList_Destroy(&test_list);
    parcJSON_Release(&json);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_ToFromJSON)
{
    CPIAddressList *truth_list = cpiAddressList_Create();
    int loops = 2;
    for (int i = 0; i < loops; i++) {
        cpiAddressList_Append(truth_list, cpiAddress_CreateFromInterface(i));
    }

    PARCJSONArray *json = cpiAddressList_ToJson(truth_list);

    CPIAddressList *test_list = cpiAddressList_CreateFromJson(json);

    assertTrue(cpiAddressList_Equals(truth_list, test_list), "Lists did not match!");

    cpiAddressList_Destroy(&truth_list);
    cpiAddressList_Destroy(&test_list);
    parcJSONArray_Release(&json);
}

LONGBOW_TEST_CASE(Global, cpiAddressList_ToString)
{
    CPIAddressList *truth_list = cpiAddressList_Create();
    int loops = 2;
    for (int i = 0; i < loops; i++) {
        cpiAddressList_Append(truth_list, cpiAddress_CreateFromInterface(i));
    }

    uint32_t beforeMemory = parcMemory_Outstanding();
    char *string = cpiAddressList_ToString(truth_list);
    assertNotNull(string, "Got null string from ToString");
    parcMemory_Deallocate((void **) &string);
    uint32_t afterMemory = parcMemory_Outstanding();

    cpiAddressList_Destroy(&truth_list);

    assertTrue(beforeMemory == afterMemory, "Memory leak from ToString by %d allocations", (int) (afterMemory - beforeMemory));
}

// ========================================

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, _cpiAddressList_FreeAddress);
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

LONGBOW_TEST_CASE(Local, _cpiAddressList_FreeAddress)
{
    CPIAddress *address = cpiAddress_CreateFromInterface(1);
    _cpiAddressList_FreeAddress((void **) &address);

    assertTrue(parcMemory_Outstanding() == 0, "Got memory imbalance: %u", parcMemory_Outstanding());
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_AddressList);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
