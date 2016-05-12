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
 * @author Glenn Scott, Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Runner.
#include "../cpi_ControlMessage.c"

#include <stdio.h>

#include <arpa/inet.h>
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <ccnx/api/control/cpi_InterfaceIPTunnel.h>

LONGBOW_TEST_RUNNER(cpi_ControlMessage)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_ControlMessage)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_ControlMessage)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_AcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateAddRouteRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateAddRouteToSelfRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateCPIRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateCancelFlowRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateConnectionListRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateIPTunnelRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateInterfaceListRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreatePauseInputRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateRemoveRouteRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateRemoveRouteToSelfRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_CreateRouteListRequest);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_Display);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_GetAckOriginalSequenceNumber);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_GetJson);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_GetNotifyStatus);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_IsACK);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_IsCPI);
    LONGBOW_RUN_TEST_CASE(Global, ccnxControl_IsNotification);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, ccnxControl_AcquireRelease)
{
    CCNxControl *control = ccnxControl_CreateInterfaceListRequest();
    CCNxControl *reference = ccnxControl_Acquire(control);

    ccnxControl_Release(&control);

    assertNull(control, "Expected control to be null");
    assertNotNull(reference, "Expected acquired reference to be non null");
    ccnxControl_Release(&reference);
    assertNull(reference, "Expected reference to be null");
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateAddRouteRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CPIRouteEntry *route = cpiRouteEntry_CreateRouteToSelf(name);
    CCNxControl *control = ccnxControl_CreateAddRouteRequest(route);

    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_REGISTER_PREFIX,
               "Expected operation %d got %d", CPI_REGISTER_PREFIX, cpi_getCPIOperation2(json));
    ccnxControl_Release(&control);
    ccnxName_Release(&name);
    cpiRouteEntry_Destroy(&route);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateAddRouteToSelfRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CCNxControl *control = ccnxControl_CreateAddRouteToSelfRequest(name);

    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_REGISTER_PREFIX,
               "Expected operation %d got %d", CPI_REGISTER_PREFIX, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
    ccnxName_Release(&name);
}


LONGBOW_TEST_CASE(Global, ccnxControl_CreateCancelFlowRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CCNxControl *control = ccnxControl_CreateCancelFlowRequest(name);
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_CANCEL_FLOW,
               "Expected operation %d got %d", CPI_CANCEL_FLOW, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
    ccnxName_Release(&name);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreatePauseInputRequest)
{
    CCNxControl *control = ccnxControl_CreatePauseInputRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_PAUSE,
               "Expected operation %d got %d", CPI_PAUSE, cpi_getCPIOperation2(json));
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateConnectionListRequest)
{
    CCNxControl *control = ccnxControl_CreateConnectionListRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_CONNECTION_LIST,
               "Expected operation %d got %d", CPI_CONNECTION_LIST, cpi_getCPIOperation2(json));
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateIPTunnelRequest)
{
    struct sockaddr_in sockaddr_any;
    memset(&sockaddr_any, 0, sizeof(sockaddr_any));
    sockaddr_any.sin_family = PF_INET;
    sockaddr_any.sin_addr.s_addr = INADDR_ANY;

    CPIAddress *source = cpiAddress_CreateFromInet(&sockaddr_any);

    struct sockaddr_in sockaddr_dst;
    memset(&sockaddr_dst, 0, sizeof(sockaddr_dst));
    sockaddr_dst.sin_family = PF_INET;
    sockaddr_dst.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &(sockaddr_dst.sin_addr));

    CPIAddress *destination = cpiAddress_CreateFromInet(&sockaddr_dst);

    CPIInterfaceIPTunnel *tunnel = cpiInterfaceIPTunnel_Create(0, source, destination, IPTUN_TCP, "tun0");
    CCNxControl *control = ccnxControl_CreateIPTunnelRequest(tunnel);

    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_CREATE_TUNNEL,
               "Expected operation %d got %d", CPI_CREATE_TUNNEL, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
    cpiInterfaceIPTunnel_Release(&tunnel);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateInterfaceListRequest)
{
    CCNxControl *control = ccnxControl_CreateInterfaceListRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_INTERFACE_LIST,
               "Expected operation %d got %d", CPI_INTERFACE_LIST, cpi_getCPIOperation2(json));
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateRemoveRouteRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CPIRouteEntry *route = cpiRouteEntry_CreateRouteToSelf(name);
    CCNxControl *control = ccnxControl_CreateRemoveRouteRequest(route);

    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_UNREGISTER_PREFIX,
               "Expected operation %d got %d", CPI_UNREGISTER_PREFIX, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
    ccnxName_Release(&name);
    cpiRouteEntry_Destroy(&route);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateRemoveRouteToSelfRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CCNxControl *control = ccnxControl_CreateRemoveRouteToSelfRequest(name);

    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_UNREGISTER_PREFIX,
               "Expected operation %d got %d", CPI_UNREGISTER_PREFIX, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
    ccnxName_Release(&name);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateRouteListRequest)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    assertTrue(cpi_getCPIOperation2(json) == CPI_PREFIX_REGISTRATION_LIST,
               "Expected operation %d got %d", CPI_PREFIX_REGISTRATION_LIST, cpi_getCPIOperation2(json));

    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_CreateCPIRequest)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    PARCJSON *cpiRequest = cpiCancelFlow_CreateRequest(name);

    CCNxControl *control = ccnxControl_CreateCPIRequest(cpiRequest);

    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    parcJSON_Release(&cpiRequest);
    ccnxControl_Release(&control);
    ccnxName_Release(&name);
}

LONGBOW_TEST_CASE(Global, ccnxControl_Display)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    ccnxControl_Display(control, 4);
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_GetAckOriginalSequenceNumber)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    PARCJSON *jsonAck = cpiAcks_CreateAck(json);

    CCNxControl *response = ccnxControl_CreateCPIRequest(jsonAck);

    // Calling GetAckOriginalSequenceNumber() to make sure the path works. We don't care
    // about the value.
    /*uint64_t originalSequenceNumber = */ ccnxControl_GetAckOriginalSequenceNumber(response);

    ccnxControl_Release(&control);
    ccnxControl_Release(&response);
    parcJSON_Release(&jsonAck);
}

LONGBOW_TEST_CASE(Global, ccnxControl_GetJson)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    PARCJSON *json = ccnxControl_GetJson(control);
    assertNotNull(json, "Expected some JSON");
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_IsACK)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    assertNotNull(control, "Expected control message to be non null");
    assertTrue(ccnxControl_IsCPI(control), "Expected control to be a CPI control message");

    PARCJSON *json = ccnxControl_GetJson(control);
    PARCJSON *jsonAck = cpiAcks_CreateAck(json);

    CCNxControl *response = ccnxControl_CreateCPIRequest(jsonAck);

    assertTrue(ccnxControl_IsACK(response), "Expected the message to be an Ack");

    ccnxControl_Release(&control);
    ccnxControl_Release(&response);
    parcJSON_Release(&jsonAck);
}

LONGBOW_TEST_CASE(Global, ccnxControl_IsCPI)
{
    CCNxControl *control = ccnxControl_CreateRouteListRequest();
    assertTrue(ccnxControl_IsCPI(control), "Expected a CPI Message");
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, ccnxControl_IsNotification)
{
    PARCJSON *json = parcJSON_Create();
    CCNxMetaMessage *notification = ccnxControlFacade_CreateNotification(json);
    CCNxControl *control = ccnxMetaMessage_GetControl(notification);

    assertTrue(ccnxControl_IsNotification(control), "Expected a notification");
    assertFalse(ccnxControl_IsCPI(control), "Did not expect a CPI command");

    parcJSON_Release(&json);
    ccnxTlvDictionary_Release(&notification);
}

LONGBOW_TEST_CASE(Global, ccnxControl_GetNotifyStatus)
{
    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");

    NotifyStatus *expected = notifyStatus_Create(1, notifyStatusCode_CONNECTION_OPEN, name, "There's a spider behind you.");

    PARCJSON *json = notifyStatus_ToJSON(expected);
    CCNxMetaMessage *notification = ccnxControlFacade_CreateNotification(json);
    CCNxControl *control = ccnxMetaMessage_GetControl(notification);

    assertTrue(ccnxControl_IsNotification(control), "Expected a notification");
    NotifyStatus *status = ccnxControl_GetNotifyStatus(control);

    assertTrue(ccnxName_Equals(notifyStatus_GetName(expected), notifyStatus_GetName(status)), "Expected equal names");
    assertTrue(notifyStatus_GetStatusCode(expected) == notifyStatus_GetStatusCode(status), "Expected equal status codes");
    assertTrue(strcmp(notifyStatus_GetMessage(expected), notifyStatus_GetMessage(status)) == 0, "Expected equal messages");

    parcJSON_Release(&json);
    ccnxTlvDictionary_Release(&notification);
    ccnxName_Release(&name);
    notifyStatus_Release(&status);
    notifyStatus_Release(&expected);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_ControlMessage);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
