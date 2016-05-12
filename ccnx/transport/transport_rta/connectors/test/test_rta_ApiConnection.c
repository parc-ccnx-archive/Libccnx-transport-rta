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
/**
 * Create a non-threaded framework to test internal RTA functions
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../rta_ApiConnection.c"

#include <poll.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include <ccnx/transport/transport_rta/config/config_All.h>
#include <ccnx/transport/transport_rta/core/rta_Framework_Commands.c>

#include <ccnx/transport/test_tools/traffic_tools.h>

typedef struct test_data {
    PARCRingBuffer1x1 *commandRingBuffer;
    PARCNotifier *commandNotifier;
    RtaFramework *framework;

    int api_fds[2];
    int stackId;

    RtaProtocolStack *stack;
    RtaConnection *connection;
} TestData;

static TestData *
_commonSetup(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    data->commandRingBuffer = parcRingBuffer1x1_Create(128, NULL);
    data->commandNotifier = parcNotifier_Create();
    data->framework = rtaFramework_Create(data->commandRingBuffer, data->commandNotifier);
    assertNotNull(data->framework, "rtaFramework_Create returned null");

    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();

    apiConnector_ProtocolStackConfig(stackConfig);
    testingLower_ProtocolStackConfig(stackConfig);
    protocolStack_ComponentsConfigArgs(stackConfig, apiConnector_GetName(), testingLower_GetName(), NULL);

    rtaFramework_NonThreadedStepCount(data->framework, 10);
    
    // Create the protocol stack

    data->stackId = 1;
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(data->stackId, stackConfig);
    _rtaFramework_ExecuteCreateStack(data->framework, createStack);
    rtaCommandCreateProtocolStack_Release(&createStack);

    rtaFramework_NonThreadedStepCount(data->framework, 10);
    data->stack = (rtaFramework_GetProtocolStackByStackId(data->framework, data->stackId))->stack;

    // Create a connection in the stack

    int error = socketpair(AF_UNIX, SOCK_STREAM, 0, data->api_fds);
    assertFalse(error, "Error creating socket pair: (%d) %s", errno, strerror(errno));

    CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
    apiConnector_ConnectionConfig(connConfig);

    tlvCodec_ConnectionConfig(connConfig);

    testingLower_ConnectionConfig(connConfig);

    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(data->stackId, data->api_fds[PAIR_OTHER], data->api_fds[PAIR_TRANSPORT], ccnxConnectionConfig_GetJson(connConfig));
    _rtaFramework_ExecuteOpenConnection(data->framework, openConnection);
    rtaCommandOpenConnection_Release(&openConnection);

    rtaFramework_NonThreadedStepCount(data->framework, 10);

    data->connection = rtaConnectionTable_GetByApiFd(data->framework->connectionTable, data->api_fds[PAIR_OTHER]);

    // cleanup

    ccnxConnectionConfig_Destroy(&connConfig);
    ccnxStackConfig_Release(&stackConfig);

    return data;
}

static void
_commonTeardown(TestData *data)
{

    rtaFramework_Teardown(data->framework);

    parcRingBuffer1x1_Release(&data->commandRingBuffer);
    parcNotifier_Release(&data->commandNotifier);
    rtaFramework_Destroy(&data->framework);

    close(data->api_fds[0]);
    close(data->api_fds[1]);
    parcMemory_Deallocate((void **) &data);
}

LONGBOW_TEST_RUNNER(rta_ApiConnection)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_ApiConnection)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_ApiConnection)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_BlockDown);
    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_Create_Checks);
    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_Create_Check_ApiSocket);

    // TODO: I don't know if this should be in or out based on what got merged together
    // This test is not new timey dictionary compatable.  See case 900.
    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_SendToApi);

    LONGBOW_RUN_TEST_CASE(Global, rtaApiConnection_UnblockDown);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    longBowTestCase_SetClipBoardData(testCase, _commonSetup());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    printf("Finishing testcase %s\n", longBowTestCase_GetName(testCase));
    _commonTeardown(longBowTestCase_GetClipBoardData(testCase));
    
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_SendToApi)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaApiConnection *apiConnection = rtaConnection_GetPrivateData(data->connection, API_CONNECTOR);

    TransportMessage *tm = trafficTools_CreateTransportMessageWithDictionaryInterest(data->connection, CCNxTlvDictionary_SchemaVersion_V1);

    RtaComponentStats *stats = rtaConnection_GetStats(data->connection, API_CONNECTOR);
    rtaApiConnection_SendToApi(apiConnection, tm, stats);
    rtaFramework_NonThreadedStepCount(data->framework, 10);

    // Let the dispatcher run
    struct pollfd pfd = { .fd = data->api_fds[PAIR_OTHER], .events = POLLIN, .revents = 0 };
    int millisecondTimeout = 1000;

    int pollvalue = poll(&pfd, 1, millisecondTimeout);
    assertTrue(pollvalue == 1, "Did not get an event from the API's side of the socket");

    CCNxMetaMessage *testMessage;
    ssize_t bytesRead = read(data->api_fds[PAIR_OTHER], &testMessage, sizeof(testMessage));
    assertTrue(bytesRead == sizeof(testMessage), "Wrong read size, got %zd expected %zd: (%d) %s",
               bytesRead, sizeof(testMessage),
               errno, strerror(errno));
    assertNotNull(testMessage, "Message read is NULL");


    assertTrue(testMessage == transportMessage_GetDictionary(tm),
               "Got wrong raw message, got %p expected %p",
               (void *) testMessage, (void *) transportMessage_GetDictionary(tm));

    ccnxMetaMessage_Release(&testMessage);
    transportMessage_Destroy(&tm);
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_BlockDown)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaApiConnection *apiConnection = rtaApiConnection_Create(data->connection);

    // make sure we're startined unblocked
    short enabled = parcEventQueue_GetEnabled(apiConnection->bev_api);
    assertTrue(enabled & PARCEventType_Read, "PARCEventType_Read is not enabled on a new Api Connector: enabled = %04X", enabled);

    rtaApiConnection_BlockDown(apiConnection);
    enabled = parcEventQueue_GetEnabled(apiConnection->bev_api);
    assertFalse(enabled & PARCEventType_Read, "PARCEventType_Read is still enabled after caling BlockDown: enabled = %04X", enabled);

    rtaApiConnection_Destroy(&apiConnection);
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_Create_Destroy)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    uint32_t beforeBalance = parcMemory_Outstanding();
    RtaApiConnection *apiConnection = rtaApiConnection_Create(data->connection);
    assertNotNull(apiConnection, "Got null API connection");

    rtaApiConnection_Destroy(&apiConnection);
    assertNull(apiConnection, "Destroy did not null apiConnection");
    uint32_t afterBalance = parcMemory_Outstanding();
    assertTrue(beforeBalance == afterBalance, "Memory imbalance: %d", (int) (afterBalance - beforeBalance));
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_Create_Checks)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    RtaApiConnection *apiConnection = rtaApiConnection_Create(data->connection);
    assertTrue(apiConnection->api_fd == rtaConnection_GetApiFd(data->connection),
               "Wrong api fd, got %d expected %d",
               apiConnection->api_fd, rtaConnection_GetApiFd(data->connection));

    assertTrue(apiConnection->transport_fd == rtaConnection_GetTransportFd(data->connection),
               "Wrong api fd, got %d expected %d",
               apiConnection->transport_fd, rtaConnection_GetTransportFd(data->connection));

    assertTrue(apiConnection->connection == data->connection,
               "Wrong connection, got %p expected %p",
               (void *) apiConnection->connection,
               (void *) data->connection);

    rtaApiConnection_Destroy(&apiConnection);
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_Create_Check_ApiSocket)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaApiConnection *apiConnection = rtaApiConnection_Create(data->connection);

    assertNotNull(apiConnection->bev_api, "API socket event null");

    rtaApiConnection_Destroy(&apiConnection);
}

LONGBOW_TEST_CASE(Global, rtaApiConnection_UnblockDown)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaApiConnection *apiConnection = rtaApiConnection_Create(data->connection);

    rtaApiConnection_BlockDown(apiConnection);
    // we know from previous test that this puts the apiConnector in blocked state

    rtaApiConnection_UnblockDown(apiConnection);
    short enabled = parcEventQueue_GetEnabled(apiConnection->bev_api);
    assertTrue(enabled & PARCEventType_Read, "PARCEventType_Read is not enabled after caling UnlockDown: enabled = %04X", enabled);

    rtaApiConnection_Destroy(&apiConnection);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_ApiConnection);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
