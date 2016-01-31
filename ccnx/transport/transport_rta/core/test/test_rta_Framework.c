/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
/**
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../rta_Framework.c"
#include <ccnx/transport/transport_rta/commands/rta_Command.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include <math.h>

typedef struct test_data {
    PARCRingBuffer1x1 *commandRingBuffer;
    PARCNotifier *commandNotifier;
    RtaFramework *framework;
} TestData;


static TestData *
_createTestData(void)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));
    data->commandRingBuffer = parcRingBuffer1x1_Create(128, NULL);
    data->commandNotifier = parcNotifier_Create();
    data->framework = rtaFramework_Create(data->commandRingBuffer, data->commandNotifier);
    rtaLogger_SetLogLevel(data->framework->logger, RtaLoggerFacility_Framework, PARCLogLevel_Debug);
    return data;
}

static void
_destroyTestData(TestData *data)
{
    parcRingBuffer1x1_Release(&data->commandRingBuffer);
    parcNotifier_Release(&data->commandNotifier);
    rtaFramework_Destroy(&data->framework);
    parcMemory_Deallocate((void **) &data);
}

LONGBOW_TEST_RUNNER(rta_Framework)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_Framework)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_Framework)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// ===================================================================

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaFramework_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, rtaFramework_GetEventScheduler);
    LONGBOW_RUN_TEST_CASE(Global, rtaFramework_GetNextConnectionId);
    LONGBOW_RUN_TEST_CASE(Global, rtaFramework_GetStatus);
    LONGBOW_RUN_TEST_CASE(Global, rtaFramework_Start_Shutdown);
    LONGBOW_RUN_TEST_CASE(Global, tick_cb);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    longBowTestCase_SetClipBoardData(testCase, _createTestData());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    _destroyTestData(longBowTestCase_GetClipBoardData(testCase));

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, rtaFramework_Create_Destroy)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    assertNotNull(data->framework, "rtaFramework_Create returned null");
    assertTrue(data->framework->commandRingBuffer == data->commandRingBuffer, "framework commandRingBuffer incorrect");
    assertTrue(data->framework->commandNotifier == data->commandNotifier, "framework commandNotifier incorrect");
    assertNotNull(data->framework->commandEvent, "framework commandEvent is null");
}

LONGBOW_TEST_CASE(Global, rtaFramework_GetEventScheduler)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    assertTrue(rtaFramework_GetEventScheduler(data->framework) == data->framework->base, "getEventScheduler broken");
}

LONGBOW_TEST_CASE(Global, rtaFramework_GetNextConnectionId)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    assertTrue(rtaFramework_GetNextConnectionId(data->framework) == 1, "GetNextConnetionId not starting at 1");
    assertTrue(rtaFramework_GetNextConnectionId(data->framework) == 2, "GetNextConnetionId first increment not 2");
}

LONGBOW_TEST_CASE(Global, rtaFramework_GetStatus)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    assertTrue(rtaFramework_GetStatus(data->framework) == FRAMEWORK_INIT, "Wrong initial status");
}

LONGBOW_TEST_CASE(Global, rtaFramework_Start_Shutdown)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    rtaFramework_Start(data->framework);
    assertTrue(rtaFramework_WaitForStatus(data->framework, FRAMEWORK_RUNNING) == FRAMEWORK_RUNNING, "Status not RUNNING");

    // blocks until done
    rtaFramework_Shutdown(data->framework);
}

LONGBOW_TEST_CASE(Global, tick_cb)
{
    ticks tic0, tic1;
    struct timeval t0, t1;
    double delta_tic, delta_t, delta_abs;

    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    rtaFramework_Start(data->framework);
    assertTrue(rtaFramework_WaitForStatus(data->framework, FRAMEWORK_RUNNING) == FRAMEWORK_RUNNING, "Status not RUNNING");

    gettimeofday(&t0, NULL);
    tic0 = data->framework->clock_ticks;
    sleep(2);
    gettimeofday(&t1, NULL);
    tic1 = data->framework->clock_ticks;

    delta_t = (t1.tv_sec + t1.tv_usec * 1E-6) - (t0.tv_sec + t0.tv_usec * 1E-6);
    delta_tic = ((tic1 - tic0) * FC_USEC_PER_TICK) * 1E-6;
    delta_abs = fabs(delta_tic - delta_t);

    printf("over 2 seconds, absolute clock error is %.6f seconds\n", delta_abs);

    // Put this test in its own WARNING only fixture on failure.  See case 983.
    //assertTrue( delta_abs < 1.0, "clock off by more than 1000 msec over 2 seconds: %.3f", delta_abs);

    // blocks until done
    rtaFramework_Shutdown(data->framework);
}

// ===================================================

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_All);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_All_Framework);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_Framework);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_ApiConnector);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_FlowController);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_Codec);
    LONGBOW_RUN_TEST_CASE(Local, _setLogLevels_ForwarderConnector);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    longBowTestCase_SetClipBoardData(testCase, _createTestData());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    _destroyTestData(longBowTestCase_GetClipBoardData(testCase));

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, _setLogLevels_All)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_All", "Warning", 1);
    _setLogLevels(data->framework);

    for (int i = 0; i < RtaLoggerFacility_END; i++) {
        bool isLoggable = rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), i, PARCLogLevel_Warning);
        assertTrue(isLoggable, "Facility %s not set to Warning", rtaLogger_FacilityString(i));
    }

    unsetenv("RtaFacility_All");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_All_Framework)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_All", "Info", 1);
    setenv("RtaFacility_Framework", "Warning", 1);
    _setLogLevels(data->framework);

    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_ApiConnector, PARCLogLevel_Info), "Api facility not Info");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Framework, PARCLogLevel_Warning), "Framework not Warning");

    unsetenv("RtaFacility_All");
    unsetenv("RtaFacility_Framework");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_Framework)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_Framework", "Warning", 1);
    _setLogLevels(data->framework);

    assertFalse(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Framework, PARCLogLevel_Info), "Info should not be loggable");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Framework, PARCLogLevel_Warning), "Warning should be loggable");
    unsetenv("RtaFacility_Framework");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_ApiConnector)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_Api", "Warning", 1);
    _setLogLevels(data->framework);

    assertFalse(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_ApiConnector, PARCLogLevel_Info), "Info should not be loggable");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_ApiConnector, PARCLogLevel_Warning), "Warning should be loggable");
    unsetenv("RtaFacility_Api");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_FlowController)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_Flowcontrol", "Warning", 1);
    _setLogLevels(data->framework);

    assertFalse(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Flowcontrol, PARCLogLevel_Info), "Info should not be loggable");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Flowcontrol, PARCLogLevel_Warning), "Warning should be loggable");
    unsetenv("RtaFacility_Flowcontrol");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_Codec)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_Codec", "Warning", 1);
    _setLogLevels(data->framework);

    assertFalse(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Codec, PARCLogLevel_Info), "Info should not be loggable");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_Codec, PARCLogLevel_Warning), "Warning should be loggable");
    unsetenv("RtaFacility_Codec");
}

LONGBOW_TEST_CASE(Local, _setLogLevels_ForwarderConnector)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    setenv("RtaFacility_Forwarder", "Warning", 1);
    _setLogLevels(data->framework);

    assertFalse(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_ForwarderConnector, PARCLogLevel_Info), "Info should not be loggable");
    assertTrue(rtaLogger_IsLoggable(rtaFramework_GetLogger(data->framework), RtaLoggerFacility_ForwarderConnector, PARCLogLevel_Warning), "Warning should be loggable");
    unsetenv("RtaFacility_Forwarder");
}

// ===================================================

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_Framework);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
