/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../rta_Commands.c"

#include "../core/components.h"
#include <ccnx/transport/transport_rta/config/config_All.h>
#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include <strings.h>

static TransportConfig *
Test_createParams(const char *local_name)
{
    assertNotNull(local_name, "%s got null keystore name\n", __func__);

    ConnectionConfig *connConfig = apiConnector_ConnectionConfig(localForwarder_ConnectionConfig(ccnxConnectionConfig_Create(), local_name));

    CCNxStackConfig *stackConfig = apiConnector_ProtocolStackConfig(
                    localForwarder_ProtocolStackConfig(
                        protocolStack_ComponentsConfigArgs(
                            ccnxStackConfig_Create(), apiConnector_GetName(), localForwarder_GetName(), NULL)));

    return transportConfig_Create(stackConfig, connConfig);
}

LONGBOW_TEST_RUNNER(rta_Commands)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_Commands)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_Commands)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Close);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateStack);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_DestroyStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetClose);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetCreateStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetDestroyStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetOpen);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetType);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Open);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Read_Write);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Shutdown);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateTransmitStatistics);
    LONGBOW_RUN_TEST_CASE(Global, CommandTransmitStatistics_FromJSON);
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

LONGBOW_TEST_CASE(Global, rtaCommand_Close)
{
    CommandClose commmandClose = { .api_fd = 7 };
    char *truth = "{ \"RTA\" : { \"CLOSE\" : 7 } }";
    PARCJSON *truth_json = parcJSON_ParseString(truth);
    char *truth_formatted = ccnxJson_ToString(truth_json);
    char *test;

    RtaCommand *command = rtaCommand_Close(commmandClose);

    assertTrue(command->type == RTA_COMMAND_CLOSE, "Type not RTA_COMMAND_CLOSE");

    test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateStack)
{
    TransportConfig *params = Test_createParams("/tmp/fwd");
    CCNxStackConfig *stackConfig = transportConfig_GetProtocolStackConfig(params);
    char *params_str = ccnxJson_ToString(ccnxStackConfig_GetJson(stackConfig));

    CommandCreateStack commandCreateStack = { .stack_id = 9, .params = ccnxStackConfig_GetJson(stackConfig) };
    char *truth = "{ \"RTA\" : { \"CREATE STACK\" : 9, \"PARAMS\" : %s } }";
    char buffer[1024];

    sprintf(buffer, truth, params_str);

    PARCJSON *truth_json = parcJSON_ParseString(buffer);
    char *truth_formatted = ccnxJson_ToString(truth_json);
    char *test;

    RtaCommand *command = rtaCommand_CreateStack(commandCreateStack);

    assertTrue(command->type == RTA_COMMAND_CREATESTACK, "Type not RTA_COMMAND_CREATESTACK");

    test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    transportConfig_Destroy(&params);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
    parcMemory_Deallocate((void **) &params_str);
}

LONGBOW_TEST_CASE(Global, rtaCommand_DestroyStack)
{
    CommandDestroyStack commandDestroyStack = { .stack_id = 2 };
    char *truth = "{ \"RTA\" : { \"DESTROY STACK\" : 2 } }";
    PARCJSON *truth_json = parcJSON_ParseString(truth);
    char *truth_formatted = ccnxJson_ToString(truth_json);
    char *test;

    RtaCommand *command = rtaCommand_DestroyStack(commandDestroyStack);

    assertTrue(command->type == RTA_COMMAND_DESTROYSTACK, "Type not RTA_COMMAND_DESTROYSTACK");

    test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetClose)
{
    CommandClose commmandClose = { .api_fd = 7 };
    CommandClose test;

    RtaCommand *command = rtaCommand_Close(commmandClose);
    rtaCommand_GetClose(command, &test);
    assertTrue(memcmp(&commmandClose, &test, sizeof(CommandClose)) == 0, "structures do not match");
    rtaCommand_Destroy(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetCreateStack)
{
    TransportConfig *params = Test_createParams("/tmp/fwd");
    CCNxStackConfig *stackConfig = transportConfig_GetProtocolStackConfig(params);

    CommandCreateStack commandCreateStack = { .stack_id = 9, .params = ccnxStackConfig_GetJson(stackConfig) };
    CommandCreateStack test;

    RtaCommand *command = rtaCommand_CreateStack(commandCreateStack);
    rtaCommand_GetCreateStack(command, &test);

    assertTrue(test.stack_id == 9, "Wrong stack id, expected %d got %d", 9, test.stack_id);

    char *truth_params = ccnxJson_ToString(ccnxStackConfig_GetJson(stackConfig));
    char *test_params = ccnxJson_ToString(test.params);
    assertTrue(strcasecmp(truth_params, test_params) == 0, "params strings did not match");

    parcMemory_Deallocate((void **) &truth_params);
    parcMemory_Deallocate((void **) &test_params);
    rtaCommand_Destroy(&command);
    transportConfig_Destroy(&params);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetDestroyStack)
{
    CommandDestroyStack commandDestroyStack = { .stack_id = 133434 };
    CommandDestroyStack test;

    RtaCommand *command = rtaCommand_DestroyStack(commandDestroyStack);
    rtaCommand_GetDestroyStack(command, &test);
    assertTrue(memcmp(&commandDestroyStack, &test, sizeof(CommandDestroyStack)) == 0, "structures do not match");
    rtaCommand_Destroy(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetOpen)
{
    TransportConfig *params = Test_createParams("/tmp/fwd");
    CCNxStackConfig *stackConfig = transportConfig_GetProtocolStackConfig(params);

    CommandOpen commandOpen = { .stack_id = 9, .api_fd = 77, .transport_fd = 102, .params = ccnxStackConfig_GetJson(stackConfig) };
    CommandOpen test;
    RtaCommand *command = rtaCommand_Open(commandOpen);
    rtaCommand_GetOpen(command, &test);

    assertTrue(test.stack_id == 9, "Wrong stack id, expected %d got %d", 9, test.stack_id);
    assertTrue(test.api_fd == 77, "Wrong api_fd, expected %d got %d", 77, test.api_fd);
    assertTrue(test.transport_fd == 102, "Wrong transport_fd, expected %d got %d", 102, test.transport_fd);

    char *truth_params = ccnxJson_ToString(ccnxStackConfig_GetJson(stackConfig));
    char *test_params = ccnxJson_ToString(test.params);
    assertTrue(strcasecmp(truth_params, test_params) == 0, "params strings did not match");

    parcMemory_Deallocate((void **) &truth_params);
    parcMemory_Deallocate((void **) &test_params);
    rtaCommand_Destroy(&command);
    transportConfig_Destroy(&params);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetType)
{
    CommandDestroyStack commandDestroyStack = { .stack_id = 2 };
    RtaCommand *command = rtaCommand_DestroyStack(commandDestroyStack);
    assertTrue(rtaCommand_GetType(command) == RTA_COMMAND_DESTROYSTACK, "Type not RTA_COMMAND_DESTROYSTACK");
    rtaCommand_Destroy(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_Open)
{
    TransportConfig *params = Test_createParams("/tmp/fwd");
    CCNxStackConfig *stackConfig = transportConfig_GetProtocolStackConfig(params);

    char *params_str = ccnxJson_ToString(ccnxStackConfig_GetJson(stackConfig));

    CommandOpen commandOpen = { .stack_id = 9, .api_fd = 77, .transport_fd = 102, .params = ccnxStackConfig_GetJson(stackConfig) };
    char *truth = "{ \"RTA\" : { \"OPEN\" : [9, 77, 102], \"PARAMS\" : %s } }";
    char buffer[1024];

    sprintf(buffer, truth, params_str);

    PARCJSON *truth_json = parcJSON_ParseString(buffer);
    char *truth_formatted = ccnxJson_ToString(truth_json);
    char *test;

    RtaCommand *command = rtaCommand_Open(commandOpen);

    assertTrue(command->type == RTA_COMMAND_OPEN, "Type not RTA_COMMAND_OPEN");

    test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    transportConfig_Destroy(&params);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
    parcMemory_Deallocate((void **) &params_str);
}

LONGBOW_TEST_CASE(Global, rtaCommand_Read_Write)
{
    int fds[2];
    pipe(fds);

    CommandDestroyStack commandDestroyStack = { .stack_id = 2 };
    RtaCommand *command = rtaCommand_DestroyStack(commandDestroyStack);
    rtaCommand_Write(command, fds[1]);
    RtaCommand *test_command = rtaCommand_Read(fds[0]);
    CommandDestroyStack test;
    rtaCommand_GetDestroyStack(test_command, &test);
    assertTrue(memcmp(&commandDestroyStack, &test, sizeof(commandDestroyStack)) == 0, "memcmp did not match");

    rtaCommand_Destroy(&command);
    rtaCommand_Destroy(&test_command);
    close(fds[1]);
    close(fds[0]);
}

LONGBOW_TEST_CASE(Global, rtaCommand_Shutdown)
{
    char *truth = "{ \"RTA\" : { \"SHUTDOWN\" : 1 } }";
    PARCJSON *truth_json = parcJSON_ParseString(truth);
    char *truth_formatted = ccnxJson_ToString(truth_json);
    char *test;

    RtaCommand *command = rtaCommand_Shutdown();

    assertTrue(command->type == RTA_COMMAND_SHUTDOWN, "Type not RTA_COMMAND_SHUTDOWN");

    test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateTransmitStatistics)
{
    char *truth = "{ \"RTA\" : { \"TransmitStatistics\" : { \"fileName\": \"/tmp/foo\", \"timeval\" : { "
                  "\"seconds\" : 1, \"microseconds\":	2 } } } }\n";

    PARCJSON *truth_json = parcJSON_ParseString(truth);
    char *truth_formatted = ccnxJson_ToString(truth_json);

    CommandTransmitStatistics transmitStatistics = {
        .timeval  = { .tv_sec = 1, .tv_usec = 2 },
        .fileName = "/tmp/foo"
    };

    RtaCommand *command = CommandTransmitStatistics_ToRtaCommand(transmitStatistics);

    assertTrue(command->type == RTA_COMMAND_TRANSMIT_STATISTICS,
               "Expected RTA_COMMAND_TRANSMIT_STATISTICS, actual %d", command->type);

    char *test = ccnxJson_ToString(command->command);
    assertTrue(strcasecmp(test, truth_formatted) == 0,
               "JSON does not match\nexpected: %s\ngot: %s\n",
               truth_formatted, test);

    rtaCommand_Destroy(&command);
    parcJSON_Release(&truth_json);
    parcMemory_Deallocate((void **) &test);
    parcMemory_Deallocate((void **) &truth_formatted);
}

LONGBOW_TEST_CASE(Global, CommandTransmitStatistics_FromJSON)
{
    CommandTransmitStatistics transmitStatistics = {
        .timeval  = { .tv_sec = 1, .tv_usec = 2 },
        .fileName = "/tmp/foo"
    };
    RtaCommand *command = CommandTransmitStatistics_ToRtaCommand(transmitStatistics);

    CommandTransmitStatistics actual;
    CommandTransmitStatistics_FromRtaCommand(command, &actual);

    assertTrue(transmitStatistics.timeval.tv_sec == actual.timeval.tv_sec, "tv_sec failed to be equal");
    assertTrue(transmitStatistics.timeval.tv_usec == actual.timeval.tv_usec, "tv_usec failed to be equal");
    assertTrue(strcmp(transmitStatistics.fileName, actual.fileName) == 0, "fileName failed to be equal");

    rtaCommand_Destroy(&command);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_Commands);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
