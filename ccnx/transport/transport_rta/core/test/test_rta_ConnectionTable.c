/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
/**
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../rta_ConnectionTable.c"
#include "../rta_ProtocolStack.c"
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(rta_ConnectionTable)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_ConnectionTable)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_ConnectionTable)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_AddConnection);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_AddConnection_TooMany);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_GetByApiFd);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_GetByTransportFd);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_Remove);
    LONGBOW_RUN_TEST_CASE(Global, rtaConnectionTable_RemoveByStack);
}

typedef struct test_data {
    PARCRingBuffer1x1 *commandRingBuffer;
    PARCNotifier *commandNotifier;

    RtaFramework *framework;

    // in some tests we use two protocol stacks
    RtaProtocolStack *stack_a;
    RtaProtocolStack *stack_b;
} TestData;

static RtaConnection *
createConnection(RtaProtocolStack *stack, int api_fd, int transport_fd)
{
    // -------
    // Create a connection to use in the table
    PARCJSON *params = parcJSON_ParseString("{}");
    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(stack->stack_id, api_fd, transport_fd, params);

    // Create a connection that goes in the connection table
    RtaConnection *conn = rtaConnection_Create(stack, openConnection);
    assertNotNull(conn, "Got null connection from rtaConnection_Create");

    rtaCommandOpenConnection_Release(&openConnection);
    parcJSON_Release(&params);
    return conn;
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    // ---------------------------
    // To test a connection table, we need to create a Framework and a Protocol stack

    data->commandRingBuffer = parcRingBuffer1x1_Create(128, NULL);
    data->commandNotifier = parcNotifier_Create();

    data->framework = rtaFramework_Create(data->commandRingBuffer, data->commandNotifier);

    // fake out a protocol stack
    data->stack_a = parcMemory_AllocateAndClear(sizeof(RtaProtocolStack));
    assertNotNull(data->stack_a, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(RtaProtocolStack));
    data->stack_a->stack_id = 1;
    data->stack_a->framework = data->framework;

    // fake out a protocol stack
    data->stack_b = parcMemory_AllocateAndClear(sizeof(RtaProtocolStack));
    assertNotNull(data->stack_b, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(RtaProtocolStack));
    data->stack_b->stack_id = 2;
    data->stack_b->framework = data->framework;

    longBowTestCase_SetClipBoardData(testCase, data);
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    // now cleanup everything
    rtaFramework_Destroy(&data->framework);
    parcNotifier_Release(&data->commandNotifier);
    parcRingBuffer1x1_Release(&data->commandRingBuffer);

    parcMemory_Deallocate((void **) &(data->stack_a));
    parcMemory_Deallocate((void **) &(data->stack_b));
    parcMemory_Deallocate((void **) &data);

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

/**
 * Destroy the table before destroying the connection
 */
LONGBOW_TEST_CASE(Global, rtaConnectionTable_AddConnection)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaConnection *conn = createConnection(data->stack_a, 2, 3);

    // This is the part we want to test.
    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);
    rtaConnectionTable_AddConnection(table, conn);

    assertTrue(table->count_elements == 1, "Incorrect table size, expected %d got %zu", 1, table->count_elements);
    rtaConnectionTable_Destroy(&table);
}


/**
 * Create a connection table with just 1 connection and make sure table
 * does the right thing on overflow
 */
LONGBOW_TEST_CASE(Global, rtaConnectionTable_AddConnection_TooMany)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaConnection *conn = createConnection(data->stack_a, 2, 3);

    int res;

    // create the table with size 1
    RtaConnectionTable *table = rtaConnectionTable_Create(1, rtaConnection_Destroy);
    res = rtaConnectionTable_AddConnection(table, conn);
    assertTrue(res == 0, "Got non-zero return %d", res);
    assertTrue(table->count_elements == 1, "Incorrect table size, expected %d got %zu", 1, table->count_elements);

    // add the second connection, should return failure
    res = rtaConnectionTable_AddConnection(table, conn);
    assertTrue(res == -1, "Should have failed, expecting -1, got %d", res);

    rtaConnectionTable_Destroy(&table);
}


LONGBOW_TEST_CASE(Global, rtaConnectionTable_Create_Destroy)
{
    size_t beforeBalance = parcMemory_Outstanding();
    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);
    assertTrue(table->max_elements == 1000, "Initialized with wrong number of elements");
    rtaConnectionTable_Destroy(&table);
    size_t afterBalance = parcMemory_Outstanding();
    assertTrue(beforeBalance == afterBalance, "Memory imbalance after create/destroy");
}

LONGBOW_TEST_CASE(Global, rtaConnectionTable_GetByApiFd)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaConnection *conn = createConnection(data->stack_a, 2, 3);

    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);
    rtaConnectionTable_AddConnection(table, conn);

    RtaConnection *test;
    test = rtaConnectionTable_GetByApiFd(table, 2);
    assertTrue(test == conn, "Got wrong connection, expecting %p got %p", (void *) conn, (void *) test);

    test = rtaConnectionTable_GetByApiFd(table, 3);
    assertTrue(test == NULL, "Got wrong connection, expecting %p got %p", NULL, (void *) test);

    test = rtaConnectionTable_GetByApiFd(table, 4);
    assertTrue(test == NULL, "Got wrong connection, expecting %p got %p", NULL, (void *) test);

    rtaConnectionTable_Destroy(&table);
}

LONGBOW_TEST_CASE(Global, rtaConnectionTable_GetByTransportFd)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaConnection *conn = createConnection(data->stack_a, 2, 3);

    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);
    rtaConnectionTable_AddConnection(table, conn);

    RtaConnection *test;
    test = rtaConnectionTable_GetByTransportFd(table, 2);
    assertTrue(test == NULL, "Got wrong connection, expecting %p got %p", NULL, (void *) test);

    test = rtaConnectionTable_GetByTransportFd(table, 3);
    assertTrue(test == conn, "Got wrong connection, expecting %p got %p", (void *) conn, (void *) test);

    test = rtaConnectionTable_GetByTransportFd(table, 4);
    assertTrue(test == NULL, "Got wrong connection, expecting %p got %p", NULL, (void *) test);


    rtaConnectionTable_Destroy(&table);
}

/**
 * We create two connections and make sure that when we remove one the other
 * is still in the table
 */
LONGBOW_TEST_CASE(Global, rtaConnectionTable_Remove)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    int res;
    int a_pair[2];
    int b_pair[2];

    // we have to use actual socket pairs in this test because Remove will destroy
    // the last copy of the connection and call close() on the sockets.
    socketpair(PF_LOCAL, SOCK_STREAM, 0, a_pair);
    socketpair(PF_LOCAL, SOCK_STREAM, 0, b_pair);

    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);

    RtaConnection *conn_a = createConnection(data->stack_a, a_pair[0], a_pair[1]);
    rtaConnectionTable_AddConnection(table, conn_a);

    RtaConnection *conn_b = createConnection(data->stack_b, b_pair[0], b_pair[1]);
    rtaConnectionTable_AddConnection(table, conn_b);

    assertTrue(table->count_elements == 2, "Wrong element count");

    res = rtaConnectionTable_Remove(table, conn_b);
    assertTrue(res == 0, "Got error from rtaConnectionTable_Remove: %d", res);
    assertTrue(table->count_elements == 1, "Wrong element count");

    RtaConnection *test = rtaConnectionTable_GetByApiFd(table, a_pair[0]);
    assertNotNull(test, "Could not retrieve connection that was supposed to still be there");

    rtaConnectionTable_Destroy(&table);
}

/**
 * Create two connections, they are in different protocol stacks.  Remove one by
 * stack id and make sure the other is still in the table
 */
LONGBOW_TEST_CASE(Global, rtaConnectionTable_RemoveByStack)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    int res;
    int a_pair[2];
    int b_pair[2];

    // we have to use actual socket pairs in this test because Remove will destroy
    // the last copy of the connection and call close() on the sockets.
    socketpair(PF_LOCAL, SOCK_STREAM, 0, a_pair);
    socketpair(PF_LOCAL, SOCK_STREAM, 0, b_pair);

    RtaConnectionTable *table = rtaConnectionTable_Create(1000, rtaConnection_Destroy);

    RtaConnection *conn_a = createConnection(data->stack_a, a_pair[0], a_pair[1]);
    rtaConnectionTable_AddConnection(table, conn_a);

    RtaConnection *conn_b = createConnection(data->stack_b, b_pair[0], b_pair[1]);
    rtaConnectionTable_AddConnection(table, conn_b);

    // now remove a connection by stack id

    res = rtaConnectionTable_RemoveByStack(table, data->stack_a->stack_id);
    assertTrue(res == 0, "Got error from rtaConnectionTable_RemoveByStack: %d", res);
    assertTrue(table->count_elements == 1, "Wrong element count");

    RtaConnection *test = rtaConnectionTable_GetByApiFd(table, b_pair[0]);
    assertNotNull(test, "Could not retrieve connection that was supposed to still be there");

    rtaConnectionTable_Destroy(&table);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_ConnectionTable);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
