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


#include "../cpi_ManageLinks.c"
#include <LongBow/testing.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_Network.h>
#include <inttypes.h>



static const short testCpiManageLinks_MetisPort = 9695;

LONGBOW_TEST_RUNNER(cpi_ManageLinks)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(cpi_ManageLinks)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(cpi_ManageLinks)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_CreateIPTunnel);
    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_CreateInterfaceListRequest);
    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_InterfacesFromControlMessage);
    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_InterfaceIPTunnelFromControlMessage);

    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_CreateConnectionListRequest);
    LONGBOW_RUN_TEST_CASE(Global, cpiLinks_ConnectionListFromControlMessage);
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

LONGBOW_TEST_CASE(Global, cpiLinks_CreateIPTunnel)
{
    // The JSON representation depends on the system sockaddr_in format, which
    // varies platform to platform. Note that the port number is encoded in the JSON,
    // so if you change the port the test will fail.
#if defined(__APPLE__)
    char *truth_format = "{\"CPI_REQUEST\":{\"SEQUENCE\":%" PRIu64 ",\"CREATE_TUNNEL\":{\"TUNNEL\":{\"IFIDX\":0,\"SYMBOLIC\":\"tun0\",\"TUNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIAAAAAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AAIl338AAAEAAAAAAAAAAA==\"}}}}}";
#elif defined(__linux__)
    char *truth_format = "{\"CPI_REQUEST\":{\"SEQUENCE\":%" PRIu64 ",\"CREATE_TUNNEL\":{\"TUNNEL\":{\"IFIDX\":0,\"SYMBOLIC\":\"tun0\",\"TUNTYPE\":\"TCP\",\"SRC\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgAAAAAAAAAAAAAAAAAAAA==\"},\"DST\":{\"ADDRESSTYPE\":\"INET\",\"DATA\":\"AgAl338AAAEAAAAAAAAAAA==\"}}}}}";
#else
    // Case 1033
    testUnimplemented("Platform not supported");
    return;
#endif

    // ---------------------------
    // Tunnel addresses
    struct sockaddr_in sockaddr_any;
    memset(&sockaddr_any, 0, sizeof(sockaddr_any));
    sockaddr_any.sin_family = PF_INET;
    sockaddr_any.sin_addr.s_addr = INADDR_ANY;

    CPIAddress *source = cpiAddress_CreateFromInet(&sockaddr_any);

    struct sockaddr_in sockaddr_dst;
    memset(&sockaddr_dst, 0, sizeof(sockaddr_dst));
    sockaddr_dst.sin_family = PF_INET;
    sockaddr_dst.sin_port = htons(testCpiManageLinks_MetisPort);
    inet_pton(AF_INET, "127.0.0.1", &(sockaddr_dst.sin_addr));

    CPIAddress *destination = cpiAddress_CreateFromInet(&sockaddr_dst);

    // ---------------------------

    CPIInterfaceIPTunnel *iptun = cpiInterfaceIPTunnel_Create(0, source, destination, IPTUN_TCP, "tun0");
    CCNxControl *control = ccnxControl_CreateIPTunnelRequest(iptun);

    char buffer[1024];
    sprintf(buffer, truth_format, cpi_GetSequenceNumber(control));

    PARCJSON *json = ccnxControl_GetJson(control);
    char *test_string = parcJSON_ToCompactString(json);
    assertTrue(strcmp(buffer, test_string) == 0, "JSON strings did not match.\nexpected %s\ngot %s\n", buffer, test_string);
    parcMemory_Deallocate((void **) &test_string);

    ccnxControl_Release(&control);
    cpiInterfaceIPTunnel_Release(&iptun);
}

LONGBOW_TEST_CASE(Global, cpiLinks_CreateInterfaceListRequest)
{
    char template[] = "{\"CPI_REQUEST\":{\"SEQUENCE\":%llu,\"INTERFACE_LIST\":{}}}";
    char truth[1024];

    CCNxControl *control = ccnxControl_CreateInterfaceListRequest();

    uint64_t seqnum = cpi_GetSequenceNumber(control);

    sprintf(truth, template, seqnum);

    PARCJSON *json = ccnxControl_GetJson(control);
    char *str = parcJSON_ToCompactString(json);
    assertTrue(strcmp(truth, str) == 0, "Did not get right json, expected '%s' got '%s'", truth, str);
    parcMemory_Deallocate((void **) &str);

    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, cpiLinks_InterfacesFromControlMessage)
{
    CCNxControl *control = ccnxControl_CreateInterfaceListRequest();

    CPIInterfaceSet *truth = cpiInterfaceSet_Create();
    CPIInterface *iface = cpiInterface_Create("eth0", 11, false, true, 1500);
    cpiInterfaceSet_Add(truth, iface);

    PARCJSON *json = cpiInterfaceSet_ToJson(truth);
    CCNxControl *response = cpi_CreateResponse(control, json);
    parcJSON_Release(&json);
    CPIInterfaceSet *test = cpiLinks_InterfacesFromControlMessage(response);

    assertTrue(cpiInterfaceSet_Equals(test, truth), "Interface sets not equal");

    ccnxControl_Release(&response);
    cpiInterfaceSet_Destroy(&truth);
    cpiInterfaceSet_Destroy(&test);
    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, cpiLinks_InterfaceIPTunnelFromControlMessage)
{
    // ---------------------------
    // Tunnel addresses
    struct sockaddr_in sockaddr_any;
    memset(&sockaddr_any, 0, sizeof(sockaddr_any));
    sockaddr_any.sin_family = PF_INET;
    sockaddr_any.sin_addr.s_addr = INADDR_ANY;

    CPIAddress *source = cpiAddress_CreateFromInet(&sockaddr_any);

    struct sockaddr_in sockaddr_dst;
    memset(&sockaddr_dst, 0, sizeof(sockaddr_dst));
    sockaddr_dst.sin_family = PF_INET;
    sockaddr_dst.sin_port = htons(testCpiManageLinks_MetisPort);
    inet_pton(AF_INET, "127.0.0.1", &(sockaddr_dst.sin_addr));

    CPIAddress *destination = cpiAddress_CreateFromInet(&sockaddr_dst);

    // ---------------------------

    CPIInterfaceIPTunnel *truth = cpiInterfaceIPTunnel_Create(0, source, destination, IPTUN_TCP, "tun0");
    CCNxControl *control = ccnxControl_CreateIPTunnelRequest(truth);

    CPIInterfaceIPTunnel *test = cpiLinks_CreateIPTunnelFromControlMessage(control);

    assertTrue(cpiInterfaceIPTunnel_Equals(truth, test), "InterfaceIPTunnels do not match");

    ccnxControl_Release(&control);
    cpiInterfaceIPTunnel_Release(&test);
    cpiInterfaceIPTunnel_Release(&truth);
}

LONGBOW_TEST_CASE(Global, cpiLinks_CreateConnectionListRequest)
{
    char template[] = "{\"CPI_REQUEST\":{\"SEQUENCE\":%llu,\"CONNECTION_LIST\":{}}}";
    char truth[1024];

    CCNxControl *control = ccnxControl_CreateConnectionListRequest();
    uint64_t seqnum = cpi_GetSequenceNumber(control);

    sprintf(truth, template, seqnum);

    PARCJSON *json = ccnxControl_GetJson(control);
    char *str = parcJSON_ToCompactString(json);

    assertTrue(strcmp(truth, str) == 0, "Did not get right json, expected '%s' got '%s'", truth, str);
    parcMemory_Deallocate((void **) &str);

    ccnxControl_Release(&control);
}

LONGBOW_TEST_CASE(Global, cpiLinks_ConnectionListFromControlMessage)
{
    // The request we'll create a response to
    CCNxControl *request = ccnxControl_CreateConnectionListRequest();

    // ---------------------------
    // Tunnel addresses
    struct sockaddr_in sockaddr_any;
    memset(&sockaddr_any, 0, sizeof(sockaddr_any));
    sockaddr_any.sin_family = PF_INET;
    sockaddr_any.sin_addr.s_addr = INADDR_ANY;

    CPIAddress *source = cpiAddress_CreateFromInet(&sockaddr_any);

    struct sockaddr_in sockaddr_dst;
    memset(&sockaddr_dst, 0, sizeof(sockaddr_dst));
    sockaddr_dst.sin_family = PF_INET;
    sockaddr_dst.sin_port = htons(testCpiManageLinks_MetisPort);
    inet_pton(AF_INET, "127.0.0.1", &(sockaddr_dst.sin_addr));

    CPIAddress *destination = cpiAddress_CreateFromInet(&sockaddr_dst);

    // ---------------------------

    CPIConnectionList *truth_list = cpiConnectionList_Create();
    cpiConnectionList_Append(truth_list, cpiConnection_Create(0, source, destination, cpiConnection_TCP));

    PARCJSON *json = cpiConnectionList_ToJson(truth_list);
    CCNxControl *response = cpi_CreateResponse(request, json);
    parcJSON_Release(&json);
    CPIConnectionList *test = cpiLinks_ConnectionListFromControlMessage(response);

    assertTrue(cpiConnectionList_Equals(truth_list, test), "InterfaceIPTunnels do not match");

    ccnxControl_Release(&response);
    ccnxControl_Release(&request);
    cpiConnectionList_Destroy(&test);
    cpiConnectionList_Destroy(&truth_list);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(cpi_ManageLinks);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
