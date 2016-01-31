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
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/un.h>
#include <strings.h>
#include <fcntl.h>
#include <limits.h>

#include <LongBow/unit-test.h>


#include <ccnx/api/control/cpi_ControlMessage.h>
#include <ccnx/common/ccnx_ContentObject.h>

#include <parc/security/parc_PublicKeySignerPkcs12Store.h>
#include <parc/security/parc_Security.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>

#include <ccnx/transport/transport_rta/config/config_All.h>
#include <ccnx/transport/transport_rta/core/rta_Framework.h>
#include <ccnx/transport/transport_rta/core/rta_Framework_Commands.c>
#include <ccnx/transport/transport_rta/core/rta_Framework_NonThreaded.h>

#include "../../test_tools/bent_pipe.h"
#include <ccnx/transport/test_tools/traffic_tools.h>

static const char local_name[] = "/tmp/beta";
static const char alice_keystore_name[] = "/tmp/alice_keystore";
static const char bob_keystore_name[] = "/tmp/bob_keystore";

static int alice_fd;
static int bob_fd;
static TransportContext *transport_context;
static CCNxTransportConfig *alice_params;
static CCNxTransportConfig *bob_params;

// reflector for FWD_LOCAL
static BentPipeState *bentpipe;

static int rnd_fd;

// for statistics
static double total_delay;
static double total_bytes_per_sec;
static unsigned item_count;

// ======================================================

static CCNxTransportConfig *
MultipleConnections_createParams(const char *local_name, const char *keystore_name, const char *keystore_passwd, const char *nonce)
{
    assertNotNull(local_name, "Got null keystore name\n");
    assertNotNull(keystore_name, "Got null keystore name\n");
    assertNotNull(keystore_passwd, "Got null keystore passwd\n");

    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();

    apiConnector_ProtocolStackConfig(
        tlvCodec_ProtocolStackConfig(
            localForwarder_ProtocolStackConfig(
                protocolStack_ComponentsConfigArgs(stackConfig,
                                                   apiConnector_GetName(),
                                                   tlvCodec_GetName(),
                                                   localForwarder_GetName(),
                                                   NULL)
                )));

    CCNxConnectionConfig *connConfig = apiConnector_ConnectionConfig(
        tlvCodec_ConnectionConfig(
            localForwarder_ConnectionConfig(ccnxConnectionConfig_Create(), local_name)));

    publicKeySignerPkcs12Store_ConnectionConfig(connConfig, keystore_name, keystore_passwd);


    // add the special nonce
    PARCJSONValue *value = parcJSONValue_CreateFromCString(nonce);
    ccnxStackConfig_Add(stackConfig, "nonce", value);
    parcJSONValue_Release(&value);

    CCNxTransportConfig *result = ccnxTransportConfig_Create(stackConfig, connConfig);
    ccnxStackConfig_Release(&stackConfig);

    return result;
}

/**
 * @function sendRandomObject
 * @abstract Sends a content object over the given socket.
 * @discussion
 *   The payload of the content object is a "struct timeval" for timing purposes.
 *
 * @param Transport socket to use
 * @return A copy of the content object sent
 */
static CCNxContentObject *
sendRandomObject(int output_fd, int fixed_size)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    if (fixed_size < (int) sizeof(tv)) {
        fixed_size = sizeof(tv);
    }

    uint8_t *buffer = parcMemory_Allocate(fixed_size);
    assertNotNull(buffer, "parcMemory_Allocate(%d) returned NULL", fixed_size);
    memcpy(buffer, (uint8_t *) &tv, sizeof(tv));

    PARCBuffer *contents = parcBuffer_Flip(parcBuffer_CreateFromArray(buffer, fixed_size));
    CCNxContentObject *object = trafficTools_CreateContentObjectWithPayload(contents);
    parcBuffer_Release(&contents);

    // Return value not checked.
    // This creates a reference to object, so we still hold the memory and can return it
    CCNxMetaMessage *meta = ccnxMetaMessage_CreateFromContentObject(object);
    Transport_Send(output_fd, meta);
    ccnxMetaMessage_Release(&meta);

    parcMemory_Deallocate((void **) &buffer);

    // truth_co and truth_msg will be freed by Transport_Send
    return object;
}

/**
 * @function recvAndCompare
 * @abstract Block on receiving a message on the input_fd, then assert its the same as the truth_obj.
 * @discussion
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 */
static bool
recvAndCompare(int input_fd, CCNxContentObject *truth_obj)
{
    struct timeval now, *then, delta;

    CCNxMetaMessage *test_msg;
    int res = Transport_Recv(input_fd, &test_msg);

    assertTrue(res == 0, "got error from Transport_Recv (%d)", res);

    // We can't directly compare the two dictionaries with CCNxTlvDictionary_Equals(),
    // because the test_obj that we read in was signed in the Transport when
    // it was sent. So the dictionaries are different.

    // So, instead, compare the payload - which should have the time at which the ContentObject
    // was created.

    CCNxContentObject *testObject = ccnxMetaMessage_GetContentObject(test_msg);
    PARCBuffer *contentsA = ccnxContentObject_GetPayload(testObject);
    PARCBuffer *contentsB = ccnxContentObject_GetPayload(truth_obj);
    assertTrue(parcBuffer_Equals(contentsA, contentsB), "Payloads do not compare");

    then = (struct timeval *) parcBuffer_Overlay(contentsA, 0);

    gettimeofday(&now, NULL);
    timersub(&now, then, &delta);

    double delay = delta.tv_sec + 1E-6 * delta.tv_usec;
    double bytes_per_sec = parcBuffer_Remaining(parcBuffer_Rewind(contentsA)) / delay;

    total_delay += delay;
    total_bytes_per_sec += bytes_per_sec;
    item_count++;

    ccnxMetaMessage_Release(&test_msg);
    return true;
}

static void
assertConnectionOpen(int fd)
{
    // wait for the CONNECTION_OPEN messages
    CCNxMetaMessage *firstMessage;
    Transport_Recv(fd, &firstMessage);

    assertTrue(ccnxMetaMessage_IsControl(firstMessage), "Expected first message to be a control message");

    CCNxControl *control = ccnxMetaMessage_GetControl(firstMessage);

    if (ccnxControl_IsNotification(control)) {
        NotifyStatus *status = ccnxControl_GetNotifyStatus(control);

        assertTrue(notifyStatus_IsConnectionOpen(status), "Expected notifyStatus_IsConnectionOpen to be true");

        notifyStatus_Release(&status);
    }

    ccnxMetaMessage_Release(&firstMessage);
}

static void
stackSetup(const char *alice_nonce, const char *bob_nonce)
{
    unlink(local_name);

    bentpipe = bentpipe_Create(local_name);
    bentpipe_SetChattyOutput(bentpipe, false);
    bentpipe_Start(bentpipe);

    transport_context = Transport_Create(TRANSPORT_RTA);

    assertNotNull(transport_context, "transportRta_Create() returned null");

    unlink(alice_keystore_name);
    unlink(bob_keystore_name);

    bool success = parcPublicKeySignerPkcs12Store_CreateFile(alice_keystore_name, "23456", "alice", 1024, 30);
    assertTrue(success, "parcPublicKeySignerPkcs12Store_CreateFile() failed.");
    success = parcPublicKeySignerPkcs12Store_CreateFile(bob_keystore_name, "34567", "bob", 2048, 15);
    assertTrue(success, "parcPublicKeySignerPkcs12Store_CreateFile() failed.");

    alice_params = MultipleConnections_createParams(local_name, alice_keystore_name, "23456", alice_nonce);
    bob_params = MultipleConnections_createParams(local_name, bob_keystore_name, "34567", bob_nonce);

    // open a connection, this will cause accpet() to fire
    alice_fd = Transport_Open(alice_params);
    bob_fd = Transport_Open(bob_params);

    assertFalse(alice_fd < 0, "Transport_Open returned error");
    assertFalse(bob_fd < 0, "Transport_Open returned error");

    assertConnectionOpen(alice_fd);
    assertConnectionOpen(bob_fd);
}

static void
stackTearDown(const char *alice_nonce, const char *bob_nonce)
{
    assertTrue(unlink(alice_keystore_name) == 0 || errno == ENOENT,
               "Unable to unlink the file %s: %s", alice_keystore_name, strerror(errno));

    assertTrue(unlink(bob_keystore_name) == 0 || errno == ENOENT,
               "Unable to unlink the file %s: %s", bob_keystore_name, strerror(errno));

    Transport_Destroy(&transport_context);
    bentpipe_Stop(bentpipe);
    bentpipe_Destroy(&bentpipe);

    ccnxTransportConfig_Destroy(&alice_params);
    ccnxTransportConfig_Destroy(&bob_params);
}

#include <parc/algol/parc_Object.h>

/**
 * @function ping
 * @abstract Send a message from one socket to another socket
 * @discussion
 *   Send a content object from one socket to another, then esure the
 *   unsigned parts of the received message compare to the sent message.
 *
 *   There's a minimum size (sizeof struct timeval).  If the fixed_size is
 *   larger than that minimum, we'll pad out to the fixed size.
 *
 * @param fixed_size is the payload content size.
 * @return <#return#>
 */
static bool
ping(int from_fd, int to_fd, int fixed_size)
{
    CCNxContentObject *object = sendRandomObject(from_fd, fixed_size);
    bool success = recvAndCompare(to_fd, object);
    assertTrue(success, "sent and received didn't compare!\n");
    ccnxContentObject_Release(&object);
    return success;
}
/**
 * use -1 for random size, othewise anything larger than 16 works
 * for the payload size
 */
static void
playPingPong(int fixed_size)
{
    int loops = 10;
    while (loops-- > 0) {
        // send down alice and up bob, then bob to alice
        ping(alice_fd, bob_fd, fixed_size);
        ping(bob_fd, alice_fd, fixed_size);
    }
}

// ======================================================


LONGBOW_TEST_RUNNER(MultipleConnections)
{
    LONGBOW_RUN_TEST_FIXTURE(SameStack);
    LONGBOW_RUN_TEST_FIXTURE(DifferentStacks);
}

LONGBOW_TEST_RUNNER_SETUP(MultipleConnections)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    rnd_fd = open("/dev/urandom", O_RDONLY);

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(MultipleConnections)
{
    close(rnd_fd);
    return LONGBOW_STATUS_SUCCEEDED;
}

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

/*
 * Same Stack tests multiple connections within the same
 * protocol stack
 */

LONGBOW_TEST_FIXTURE(SameStack)
{
    LONGBOW_RUN_TEST_CASE(SameStack, alice_bob_pingpong);
}

LONGBOW_TEST_FIXTURE_SETUP(SameStack)
{
    parcSecurity_Init();
    stackSetup("apple", "apple");

    total_delay = total_bytes_per_sec = 0.0;
    item_count = 0;
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(SameStack)
{
    longBowDebug("average delay %.6f sec, avg bytes/sec %.3f\n",
                 total_delay / item_count, total_bytes_per_sec / item_count);

    stackTearDown("apple", "apple");

    parcSecurity_Fini();

    if (parcMemory_Outstanding() != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(SameStack, alice_bob_pingpong)
{
    playPingPong(8192);
}

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

/*
 * DifferentStacks tests multiple connections through
 * different stacks in the same transport
 */

LONGBOW_TEST_FIXTURE(DifferentStacks)
{
    LONGBOW_RUN_TEST_CASE(DifferentStacks, alice_bob_pingpong);
}

LONGBOW_TEST_FIXTURE_SETUP(DifferentStacks)
{
    parcSecurity_Init();
    stackSetup("apple", "oranges");
    total_delay = total_bytes_per_sec = 0.0;
    item_count = 0;

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(DifferentStacks)
{
    stackTearDown("apple", "oranges");

    parcSecurity_Fini();

    if (parcMemory_Outstanding() != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(DifferentStacks, alice_bob_pingpong)
{
    playPingPong(-1);
}

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(MultipleConnections);
    exit(longBowMain(argc, argv, testRunner, NULL));
}
