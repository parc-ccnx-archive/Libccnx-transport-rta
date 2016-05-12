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

#include "../rta_ComponentStats.c"
#include <parc/algol/parc_SafeMemory.h>

#include <ccnx/transport/transport_rta/core/rta_ProtocolStack.h>
#include <ccnx/transport/transport_rta/config/config_All.h>

#include <inttypes.h>
#include <sys/socket.h>
#include <errno.h>

#define PAIR_OTHER 0
#define PAIR_TRANSPORT 1

#include <LongBow/unit-test.h>

typedef struct test_data {
    PARCRingBuffer1x1 *commandRingBuffer;
    PARCNotifier *commandNotifier;

    int api_fds[2];
    RtaFramework *framework;
    RtaProtocolStack *stack;
} TestData;

static TestData *
_commonSetup(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    int error = socketpair(AF_UNIX, SOCK_STREAM, 0, data->api_fds);
    assertTrue(error == 0, "Error creating socket pair: (%d) %s", errno, strerror(errno));

    data->commandRingBuffer = parcRingBuffer1x1_Create(128, NULL);
    data->commandNotifier = parcNotifier_Create();
    data->framework = rtaFramework_Create(data->commandRingBuffer, data->commandNotifier);
    assertNotNull(data->framework, "rtaFramework_Create returned null");

    rtaFramework_Start(data->framework);

    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();
    apiConnector_ProtocolStackConfig(stackConfig);
    testingLower_ProtocolStackConfig(stackConfig);
    protocolStack_ComponentsConfigArgs(stackConfig, apiConnector_GetName(), testingLower_GetName(), NULL);
    data->stack = rtaProtocolStack_Create(data->framework, ccnxStackConfig_GetJson(stackConfig), 1);

    ccnxStackConfig_Release(&stackConfig);
    return data;
}

static void
_commonTeardown(TestData *data)
{
    rtaProtocolStack_Destroy(&data->stack);

    // blocks until done
    rtaFramework_Shutdown(data->framework);

    parcRingBuffer1x1_Release(&data->commandRingBuffer);
    parcNotifier_Release(&data->commandNotifier);
    rtaFramework_Destroy(&data->framework);

    close(data->api_fds[0]);
    close(data->api_fds[1]);
    parcMemory_Deallocate((void **) &data);
}

LONGBOW_TEST_RUNNER(rta_Stats)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_Stats)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_Stats)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, stats_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, stats_Dump);
    LONGBOW_RUN_TEST_CASE(Global, stats_Get);
    LONGBOW_RUN_TEST_CASE(Global, stats_Increment);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    longBowTestCase_SetClipBoardData(testCase, _commonSetup());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    _commonTeardown(longBowTestCase_GetClipBoardData(testCase));
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, stats_Create_Destroy)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaComponentStats *stats = rtaComponentStats_Create(data->stack, API_CONNECTOR);

    assertNotNull(stats, "Got null stats from rtaComponentStats_Create");
    assertTrue(stats->stack == data->stack,
               "Bad stack pointer, got %p expected %p",
               (void *) stats->stack, (void *) data->stack);

    rtaComponentStats_Destroy(&stats);
}

LONGBOW_TEST_CASE(Global, stats_Dump)
{
    for (int i = 0; i < STATS_LAST; i++) {
        char *test = rtaComponentStatType_ToString(i);
        assertNotNull(test, "Got null string for stat type %d", i);
    }
}

LONGBOW_TEST_CASE(Global, stats_Get)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaComponentStats *stats = rtaComponentStats_Create(data->stack, API_CONNECTOR);

    for (int i = 0; i < STATS_LAST; i++) {
        // set each stat to a value
        uint64_t value = i + 5;
        stats->stats[i] = value;

        uint64_t counter = stats->stats[i];
        assertTrue(counter == value, "Counter %d wrong value, got %" PRIu64 " expected %" PRIu64, i, counter, value);
    }

    rtaComponentStats_Destroy(&stats);
}

LONGBOW_TEST_CASE(Global, stats_Increment)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    RtaComponentStats *stats = rtaComponentStats_Create(data->stack, API_CONNECTOR);

    for (int i = 0; i < STATS_LAST; i++) {
        rtaComponentStats_Increment(stats, (RtaComponentStatType) i);
    }

    // now make sure they are all "1"
    for (int i = 0; i < STATS_LAST; i++) {
        uint64_t counter = stats->stats[i];
        assertTrue(counter == 1, "Counter %d wrong value, got %" PRIu64 "expected 1", i, counter);
    }

    rtaComponentStats_Destroy(&stats);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_Stats);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
