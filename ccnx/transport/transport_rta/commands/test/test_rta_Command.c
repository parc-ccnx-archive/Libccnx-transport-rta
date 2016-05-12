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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../rta_Command.c"

#include <inttypes.h>
#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(rta_Command)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(rta_Command)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(rta_Command)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Release);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateShutdownFramework);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateCloseConnection);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateCreateProtocolStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateDestroyProtocolStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateOpenConnection);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_CreateTransmitStatistics);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetCloseConnection);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetCreateProtocolStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetDestroyProtocolStack);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetOpenConnection);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_GetTransmitStatistics);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsCloseConnection_True);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsCreateProtocolStack_True);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsDestroyProtocolStack_True);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsOpenConnection_True);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsShutdownFramework_True);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsTransmitStatistics_True);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsCloseConnection_False);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsCreateProtocolStack_False);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsDestroyProtocolStack_False);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsOpenConnection_False);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsShutdownFramework_False);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_IsTransmitStatistics_False);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Read_Single);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Write_Single);

    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Read_Underflow);
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Write_Overflow);

    // miscellaneous functions
    LONGBOW_RUN_TEST_CASE(Global, rtaCommand_Display);
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

LONGBOW_TEST_CASE(Global, rtaCommand_Acquire)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    size_t firstRefCount = parcObject_GetReferenceCount(command);

    RtaCommand *second = rtaCommand_Acquire(command);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    assertTrue(secondRefCount == firstRefCount + 1, "Wrong refcount after acquire, got %zu expected %zu", secondRefCount, firstRefCount + 1);

    rtaCommand_Release(&command);
    rtaCommand_Release(&second);
}

LONGBOW_TEST_CASE(Global, rtaCommand_Release)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();

    RtaCommand *second = rtaCommand_Acquire(command);
    size_t secondRefCount = parcObject_GetReferenceCount(second);

    rtaCommand_Release(&command);
    size_t thirdRefCount = parcObject_GetReferenceCount(second);

    assertTrue(thirdRefCount == secondRefCount - 1, "Wrong refcount after release, got %zu expected %zu", thirdRefCount, secondRefCount - 1);

    rtaCommand_Release(&second);
}

// =======================
// Create/From operations

LONGBOW_TEST_CASE(Global, rtaCommand_CreateShutdownFramework)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_ShutdownFramework, "Command is not shutdown framework");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateCloseConnection)
{
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(77);
    RtaCommand *command = rtaCommand_CreateCloseConnection(closeConnection);
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_CloseConnection, "Command is not CloseConnection");
    rtaCommand_Release(&command);
    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateCreateProtocolStack)
{
    CCNxStackConfig *config = ccnxStackConfig_Create();
    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(111, config);

    RtaCommand *command = rtaCommand_CreateCreateProtocolStack(createStack);
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_CreateProtocolStack, "Command is not CreateProtocolStack");

    rtaCommand_Release(&command);
    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateDestroyProtocolStack)
{
    RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(77);
    RtaCommand *command = rtaCommand_CreateDestroyProtocolStack(destroyStack);
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_DestroyProtocolStack, "Command is not DestroyProtocolStack");
    rtaCommand_Release(&command);
    rtaCommandDestroyProtocolStack_Release(&destroyStack);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateOpenConnection)
{
    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(111, 2341, 2450987, NULL);
    RtaCommand *command = rtaCommand_CreateOpenConnection(openConnection);
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_OpenConnection, "Command is not OpenConnection");
    rtaCommand_Release(&command);
    rtaCommandOpenConnection_Release(&openConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_CreateTransmitStatistics)
{
    RtaCommandTransmitStatistics *transmitStats = rtaCommandTransmitStatistics_Create((struct timeval) { 1, 2 }, "filename");
    RtaCommand *command = rtaCommand_CreateTransmitStatistics(transmitStats);
    assertNotNull(command, "Got null command from create");
    assertTrue(command->type == RtaCommandType_TransmitStatistics, "Command is not TransmitStatistics");
    rtaCommand_Release(&command);
    rtaCommandTransmitStatistics_Release(&transmitStats);
}

// =======================
// GET operations

LONGBOW_TEST_CASE(Global, rtaCommand_GetCloseConnection)
{
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(77);
    RtaCommand *command = rtaCommand_CreateCloseConnection(closeConnection);

    const RtaCommandCloseConnection *test = rtaCommand_GetCloseConnection(command);
    assertTrue(test == closeConnection, "Wrong pointers, got %p expected %p", (void *) test, (void *) closeConnection);

    rtaCommand_Release(&command);
    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetCreateProtocolStack)
{
    CCNxStackConfig *config = ccnxStackConfig_Create();

    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(111, config);
    RtaCommand *command = rtaCommand_CreateCreateProtocolStack(createStack);

    const RtaCommandCreateProtocolStack *test = rtaCommand_GetCreateProtocolStack(command);
    assertTrue(test == createStack, "Wrong pointers, got %p expected %p", (void *) test, (void *) createStack);

    rtaCommand_Release(&command);
    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetDestroyProtocolStack)
{
    RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(77);
    RtaCommand *command = rtaCommand_CreateDestroyProtocolStack(destroyStack);

    const RtaCommandDestroyProtocolStack *test = rtaCommand_GetDestroyProtocolStack(command);
    assertTrue(test == destroyStack, "Wrong pointers, got %p expected %p", (void *) test, (void *) destroyStack);

    rtaCommand_Release(&command);
    rtaCommandDestroyProtocolStack_Release(&destroyStack);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetOpenConnection)
{
    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(111, 2341, 2450987, NULL);
    RtaCommand *command = rtaCommand_CreateOpenConnection(openConnection);

    const RtaCommandOpenConnection *test = rtaCommand_GetOpenConnection(command);
    assertTrue(test == openConnection, "Wrong pointers, got %p expected %p", (void *) test, (void *) openConnection);

    rtaCommand_Release(&command);
    rtaCommandOpenConnection_Release(&openConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_GetTransmitStatistics)
{
    RtaCommandTransmitStatistics *transmitStats = rtaCommandTransmitStatistics_Create((struct timeval) { 1, 2 }, "filename");
    RtaCommand *command = rtaCommand_CreateTransmitStatistics(transmitStats);

    const RtaCommandTransmitStatistics *test = rtaCommand_GetTransmitStatistics(command);
    assertTrue(test == transmitStats, "Wrong pointers, got %p expected %p", (void *) test, (void *) transmitStats);

    rtaCommand_Release(&command);
    rtaCommandTransmitStatistics_Release(&transmitStats);
}

// =======================
// IsX operations

LONGBOW_TEST_CASE(Global, rtaCommand_IsCloseConnection_True)
{
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(77);
    RtaCommand *command = rtaCommand_CreateCloseConnection(closeConnection);
    assertTrue(rtaCommand_IsCloseConnection(command), "Command is not CloseConnection");
    rtaCommand_Release(&command);
    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsCreateProtocolStack_True)
{
    CCNxStackConfig *config = ccnxStackConfig_Create();

    RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(111, config);
    RtaCommand *command = rtaCommand_CreateCreateProtocolStack(createStack);
    assertTrue(rtaCommand_IsCreateProtocolStack(command), "Command is not CreateProtocolStack");
    rtaCommand_Release(&command);
    rtaCommandCreateProtocolStack_Release(&createStack);
    ccnxStackConfig_Release(&config);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsDestroyProtocolStack_True)
{
    RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(77);
    RtaCommand *command = rtaCommand_CreateDestroyProtocolStack(destroyStack);
    assertTrue(rtaCommand_IsDestroyProtocolStack(command), "Command is not DestroyProtocolStack");
    rtaCommand_Release(&command);
    rtaCommandDestroyProtocolStack_Release(&destroyStack);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsOpenConnection_True)
{
    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(111, 2341, 2450987, NULL);
    RtaCommand *command = rtaCommand_CreateOpenConnection(openConnection);
    assertTrue(rtaCommand_IsOpenConnection(command), "Command is not OpenConnection");
    rtaCommand_Release(&command);
    rtaCommandOpenConnection_Release(&openConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsShutdownFramework_True)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertTrue(rtaCommand_IsShutdownFramework(command), "Command is not shutdown framework");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsTransmitStatistics_True)
{
    RtaCommandTransmitStatistics *transmitStats = rtaCommandTransmitStatistics_Create((struct timeval) { 1, 2 }, "filename");
    RtaCommand *command = rtaCommand_CreateTransmitStatistics(transmitStats);
    assertTrue(rtaCommand_IsTransmitStatistics(command), "Command is not TransmitStatistics");
    rtaCommand_Release(&command);
    rtaCommandTransmitStatistics_Release(&transmitStats);
}


LONGBOW_TEST_CASE(Global, rtaCommand_IsCloseConnection_False)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertFalse(rtaCommand_IsCloseConnection(command), "Command is not CloseConnection, should be false");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsCreateProtocolStack_False)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertFalse(rtaCommand_IsCreateProtocolStack(command), "Command is not CreateProtocolStack, should be false");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsDestroyProtocolStack_False)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertFalse(rtaCommand_IsDestroyProtocolStack(command), "Command is not DestroyProtocolStack, should be false");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsOpenConnection_False)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertFalse(rtaCommand_IsOpenConnection(command), "Command is not OpenConnection, should be false");
    rtaCommand_Release(&command);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsShutdownFramework_False)
{
    RtaCommandCloseConnection *closeConnection = rtaCommandCloseConnection_Create(77);
    RtaCommand *command = rtaCommand_CreateCloseConnection(closeConnection);
    assertFalse(rtaCommand_IsShutdownFramework(command), "Command is not ShutdownFramework, should be false");
    rtaCommand_Release(&command);
    rtaCommandCloseConnection_Release(&closeConnection);
}

LONGBOW_TEST_CASE(Global, rtaCommand_IsTransmitStatistics_False)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    assertFalse(rtaCommand_IsTransmitStatistics(command), "Command is not TransmitStatistics, should be false");
    rtaCommand_Release(&command);
}

// ===========================
// IO operations

/*
 * Read a single command from a ring buffer
 */
LONGBOW_TEST_CASE(Global, rtaCommand_Read_Single)
{
    PARCRingBuffer1x1 *ring = parcRingBuffer1x1_Create(4, NULL);
    RtaCommand *command = rtaCommand_CreateShutdownFramework();

    bool success = parcRingBuffer1x1_Put(ring, command);
    assertTrue(success, "Failed to put command in to ring buffer");

    RtaCommand *test = rtaCommand_Read(ring);
    assertTrue(test == command, "Wrong pointers, got %p expected %p", (void *) test, (void *) command);

    rtaCommand_Release(&command);
    parcRingBuffer1x1_Release(&ring);
}

/*
 * Write a single command to a ring buffer and make sure it works
 */
LONGBOW_TEST_CASE(Global, rtaCommand_Write_Single)
{
    PARCRingBuffer1x1 *ring = parcRingBuffer1x1_Create(4, NULL);
    RtaCommand *command = rtaCommand_CreateShutdownFramework();

    bool success = rtaCommand_Write(command, ring);
    assertTrue(success, "Failed to put command in to ring buffer");

    // We should now have two references
    assertTrue(parcObject_GetReferenceCount(command) == 2, "Wrong refernce count, got %" PRIu64 " expected %u", parcObject_GetReferenceCount(command), 2);

    RtaCommand *test = rtaCommand_Read(ring);
    assertTrue(test == command, "Wrong pointers, got %p expected %p", (void *) test, (void *) command);

    rtaCommand_Release(&command);
    rtaCommand_Release(&test);
    parcRingBuffer1x1_Release(&ring);
}

/*
 * Read from an empty ring buffer
 */
LONGBOW_TEST_CASE(Global, rtaCommand_Read_Underflow)
{
    PARCRingBuffer1x1 *ring = parcRingBuffer1x1_Create(4, NULL);

    RtaCommand *test = rtaCommand_Read(ring);
    assertNull(test, "Should have gotten NULL read from an empty ring buffer");

    parcRingBuffer1x1_Release(&ring);
}

/*
 * Write beyond the capacity of the ring buffer
 */
LONGBOW_TEST_CASE(Global, rtaCommand_Write_Overflow)
{
    // The ring will store up to (ringSize-1) elements, so we can only
    // have 3 items in a ring of size 4
    unsigned ringSize = 4;
    RtaCommand *commandArray[ringSize];

    PARCRingBuffer1x1 *ring = parcRingBuffer1x1_Create(ringSize, NULL);

    for (int i = 0; i < ringSize; i++) {
        commandArray[i] = rtaCommand_CreateShutdownFramework();
    }

    for (int i = 0; i < ringSize - 1; i++) {
        bool success = rtaCommand_Write(commandArray[i], ring);
        assertTrue(success, "Failed to put command in to ring buffer");
    }

    // now put the one that will not fit
    bool shouldFail = rtaCommand_Write(commandArray[ringSize - 1], ring);
    assertFalse(shouldFail, "Writing overflow item should have failed");

    // now make sure we read off all the right items
    for (int i = 0; i < ringSize - 1; i++) {
        RtaCommand *test = rtaCommand_Read(ring);
        assertTrue(test == commandArray[i], "Wrong pointers, got %p expected %p", (void *) test, (void *) commandArray[i]);
        rtaCommand_Release(&test);
    }

    for (int i = 0; i < ringSize; i++) {
        rtaCommand_Release(&commandArray[i]);
    }
    parcRingBuffer1x1_Release(&ring);
}

LONGBOW_TEST_CASE(Global, rtaCommand_Display)
{
    RtaCommand *command = rtaCommand_CreateShutdownFramework();
    rtaCommand_Display(command, 3);
    rtaCommand_Release(&command);
}

// ===================================================================

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(rta_Command);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
