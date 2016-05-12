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

#include "../cpi_Interface.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>


LONGBOW_TEST_RUNNER(cpi_Interface)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_Interface)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_Interface)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// ==================================================================

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_AddAddress);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_GetAddresses);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_GetMtu);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_GetInterfaceIndex);

    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_NameEquals_IsEqual);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_NameEquals_IsNotEqual);

    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_ToJson);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_FromJson);

    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_IsEqual);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_BothNull);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_OneNull);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalName);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalIndex);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalLoopback);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalMulticast);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalMTU);
    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_Equals_UnequalAddresses);

    LONGBOW_RUN_TEST_CASE(Global, cpiInterface_ToString);
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

LONGBOW_TEST_CASE(Global, cpiInterface_Create_Destroy)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_Destroy(&iface);

    assertTrue(parcSafeMemory_ReportAllocation(STDOUT_FILENO) == 0, "Memory imbalance on create/destroy");
}

LONGBOW_TEST_CASE(Global, cpiInterface_AddAddress)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);

    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    assertTrue(cpiAddressList_Length(iface->addressList) == 1,
               "Incorrect address list length, expected %u got %zu",
               1,
               cpiAddressList_Length(iface->addressList));

    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(2));
    assertTrue(cpiAddressList_Length(iface->addressList) == 2,
               "Incorrect address list length, expected %u got %zu",
               2,
               cpiAddressList_Length(iface->addressList));

    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_GetAddresses)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);

    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(2));

    const CPIAddressList *list = cpiInterface_GetAddresses(iface);
    assertTrue(cpiAddressList_Length(list) == 2, "Incorrect list size, expected %u got %zu", 2, cpiAddressList_Length(list));
    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_GetMtu)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);

    unsigned test = cpiInterface_GetMTU(iface);
    assertTrue(test == 1500, "Wrong MTU expected 1500 got %u", test);
    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_GetInterfaceIndex)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);

    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(2));

    unsigned testvalue = cpiInterface_GetInterfaceIndex(iface);

    assertTrue(testvalue == 1, "Incorrect interfaceIndex, expected %u got %u", 1, testvalue);
    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_NameEquals_IsEqual)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    assertTrue(cpiInterface_NameEquals(iface, "eth0"), "name did not compare as equal");
    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_NameEquals_IsNotEqual)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    assertFalse(cpiInterface_NameEquals(iface, "eth2"), "Unequal names compare as equal");
    cpiInterface_Destroy(&iface);
}


LONGBOW_TEST_CASE(Global, cpiInterface_ToJson)
{
    char truth[] = "{\"Interface\":{\"Name\":\"eth0\",\"Index\":1,\"Loopback\":\"true\",\"Multicast\":\"false\",\"MTU\":1500,\"Addrs\":[{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAQ==\"},{\"ADDRESSTYPE\":\"IFACE\",\"DATA\":\"AAAAAg==\"}]}}";

    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(2));

    PARCJSON *json = cpiInterface_ToJson(iface);

    char *str = parcJSON_ToCompactString(json);
    assertTrue(strcmp(str, truth) == 0, "JSON mismatch, expected '%s' got '%s'", truth, str);
    parcMemory_Deallocate((void **) &str);
    parcJSON_Release(&json);
    cpiInterface_Destroy(&iface);
}

LONGBOW_TEST_CASE(Global, cpiInterface_FromJson)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(2));

    PARCJSON *json = cpiInterface_ToJson(iface);

    CPIInterface *test_iface = cpiInterface_FromJson(json);

    assertTrue(cpiInterface_Equals(iface, test_iface), "Interface from json not equal to truth");

    cpiInterface_Destroy(&test_iface);
    cpiInterface_Destroy(&iface);
    parcJSON_Release(&json);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_IsEqual)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertTrue(cpiInterface_Equals(iface_a, iface_b), "Two equal interfaces did not compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_BothNull)
{
    assertTrue(cpiInterface_Equals(NULL, NULL), "Two NULL interfaces did not compare equal");
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_OneNull)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, NULL), "One null one non-null interfaces compare equal");

    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalName)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth1", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalIndex)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 2, true, false, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalLoopback)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 1, false, false, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalMulticast)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 1, true, true, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalMTU)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 1, true, false, 9000);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_Equals_UnequalAddresses)
{
    CPIInterface *iface_a = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(1));
    cpiInterface_AddAddress(iface_a, cpiAddress_CreateFromInterface(2));

    CPIInterface *iface_b = cpiInterface_Create("eth0", 1, true, false, 1500);
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(3));
    cpiInterface_AddAddress(iface_b, cpiAddress_CreateFromInterface(2));

    assertFalse(cpiInterface_Equals(iface_a, iface_b), "Two unequal interfaces compare equal");

    cpiInterface_Destroy(&iface_b);
    cpiInterface_Destroy(&iface_a);
}

LONGBOW_TEST_CASE(Global, cpiInterface_ToString)
{
    CPIInterface *iface = cpiInterface_Create("eth0", 1, false, true, 1500);
    cpiInterface_AddAddress(iface, cpiAddress_CreateFromInterface(1));

    uint32_t beforeBalance = parcMemory_Outstanding();
    char *string = cpiInterface_ToString(iface);
    parcMemory_Deallocate((void **) &string);
    uint32_t afterBalance = parcMemory_Outstanding();
    cpiInterface_Destroy(&iface);

    assertTrue(beforeBalance == afterBalance, "Memory leak: off by %d allocations", (int) (afterBalance - beforeBalance));
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_Interface);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
