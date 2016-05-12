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

#include "../component_Codec_Tlv.c"
#include <parc/algol/parc_SafeMemory.h>

#include <stdio.h>
#include <sys/un.h>
#include <strings.h>

#include <LongBow/unit-test.h>
#include <LongBow/runtime.h>

#define DEBUG_OUTPUT 0

#include <parc/security/parc_Security.h>
#include <parc/security/parc_SymmetricKeyStore.h>

#include <ccnx/transport/transport_rta/config/config_All.h>
#include <ccnx/transport/transport_rta/core/rta_Framework.h>
#include <ccnx/transport/transport_rta/core/rta_Framework_NonThreaded.h>
#include <ccnx/transport/transport_rta/core/rta_Component.h>

#include <ccnx/transport/test_tools/traffic_tools.h>

#include "testrig_MockFramework.c"

#ifndef MAXPATH
#define MAXPATH 1024
#endif

typedef struct test_data {
    MockFramework *mock;
    char keystore_filename[MAXPATH];
    char keystore_password[MAXPATH];
} TestData;

LONGBOW_TEST_RUNNER(System)
{
    LONGBOW_RUN_TEST_FIXTURE(Component_Codec_Tlv_Hmac);
}

LONGBOW_TEST_RUNNER_SETUP(System)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(System)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// ================================

static CCNxTransportConfig *
codecTlv_CreateParams(const char *keystore_name, const char *keystore_passwd)
{
    assertNotNull(keystore_name, "Got null keystore name\n");
    assertNotNull(keystore_passwd, "Got null keystore passwd\n");

    CCNxStackConfig *stackConfig = ccnxStackConfig_Create();

    apiConnector_ProtocolStackConfig(stackConfig);
    testingUpper_ProtocolStackConfig(stackConfig);
    tlvCodec_ProtocolStackConfig(stackConfig);
    testingLower_ProtocolStackConfig(stackConfig);
    protocolStack_ComponentsConfigArgs(stackConfig,
                                       apiConnector_GetName(),
                                       testingUpper_GetName(),
                                       tlvCodec_GetName(),
                                       testingLower_GetName(),
                                       NULL);

    CCNxConnectionConfig *connConfig = apiConnector_ConnectionConfig(ccnxConnectionConfig_Create());
    testingUpper_ConnectionConfig(connConfig);
    tlvCodec_ConnectionConfig(connConfig);
    testingLower_ConnectionConfig(connConfig);

    symmetricKeySigner_ConnectionConfig(connConfig, keystore_name, keystore_passwd);
    CCNxTransportConfig *result = ccnxTransportConfig_Create(stackConfig, connConfig);
    ccnxStackConfig_Release(&stackConfig);
    return result;
}

static TestData *
_commonSetup(void)
{
    parcSecurity_Init();

    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    sprintf(data->keystore_filename, "/tmp/alice_keystore.p12.XXXXXX");
    mktemp(data->keystore_filename);
    sprintf(data->keystore_password, "12345");

    unlink(data->keystore_filename);
    PARCBuffer *secret_key = parcSymmetricKeyStore_CreateKey(256);
    parcSymmetricKeyStore_CreateFile(data->keystore_filename, data->keystore_password, secret_key);
    parcBuffer_Release(&secret_key);

    CCNxTransportConfig *config = codecTlv_CreateParams(data->keystore_filename, data->keystore_password);
    data->mock = mockFramework_Create(config);
    ccnxTransportConfig_Destroy(&config);

    return data;
}

static void
_commonTeardown(TestData *data)
{
    mockFramework_Destroy(&data->mock);
    unlink(data->keystore_filename);
    parcMemory_Deallocate((void **) &data);

    parcSecurity_Fini();
}

// ======================================


LONGBOW_TEST_FIXTURE(Component_Codec_Tlv_Hmac)
{
    LONGBOW_RUN_TEST_CASE(Component_Codec_Tlv_Hmac, open_close);
    LONGBOW_RUN_TEST_CASE(Component_Codec_Tlv_Hmac, Component_Codec_Tlv_Hmac_Downcall_Read);
}

LONGBOW_TEST_FIXTURE_SETUP(Component_Codec_Tlv_Hmac)
{
    longBowTestCase_SetClipBoardData(testCase, _commonSetup());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Component_Codec_Tlv_Hmac)
{
    _commonTeardown(longBowTestCase_GetClipBoardData(testCase));

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Component_Codec_Tlv_Hmac, open_close)
{
    // dont actually do anything.  make sure no memory leaks in setup and teardown.
}

// ============================================
// TLV tests

static TransportMessage *
sendDown(TestData *data, TransportMessage *tm_going_down)
{
    PARCEventQueue *in = rtaProtocolStack_GetPutQueue(data->mock->stack, TESTING_UPPER, RTA_DOWN);
    PARCEventQueue *out = rtaProtocolStack_GetPutQueue(data->mock->stack, TESTING_LOWER, RTA_UP);

    rtaComponent_PutMessage(in, tm_going_down);
    // turn the handle enough times, the message will pass all the way out the bottom
    rtaFramework_NonThreadedStepCount(data->mock->framework, 5);
    return rtaComponent_GetMessage(out);
}

LONGBOW_TEST_CASE(Component_Codec_Tlv_Hmac, Component_Codec_Tlv_Hmac_Downcall_Read)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    TransportMessage *tm = trafficTools_CreateTransportMessageWithSignedContentObject(data->mock->connection);

    TransportMessage *test_tm = sendDown(data, tm);

    // we should now have a RawFormat message
    CCNxCodecNetworkBufferIoVec *vec = ccnxWireFormatMessage_GetIoVec(transportMessage_GetDictionary(test_tm));
    assertNotNull(vec, "Output of coded message did not have a raw format message")
    {
        ccnxTlvDictionary_Display(transportMessage_GetDictionary(test_tm), 0);
    }

    transportMessage_Destroy(&test_tm);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(System);
    exit(longBowMain(argc, argv, testRunner, NULL));
}
