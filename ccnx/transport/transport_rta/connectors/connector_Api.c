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
 * Interface between the event dispatcher and component callbacks to
 * the RtaApiConnection.  The API connector, per se, is implemented in rta_ApiConnection.  This
 * module is the scaffolding to work within the RTA component framework.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>

#include <ccnx/transport/transport_rta/connectors/rta_ApiConnection.h>

#include <ccnx/transport/transport_rta/core/rta_Framework_Services.h>
#include <ccnx/transport/transport_rta/core/rta_ProtocolStack.h>
#include <ccnx/transport/transport_rta/core/rta_Component.h>
#include <ccnx/transport/transport_rta/connectors/connector_Api.h>

#include <ccnx/api/control/controlPlaneInterface.h>

#ifndef DEBUG_OUTPUT
#define DEBUG_OUTPUT 0
#endif

static int  connector_Api_Init(RtaProtocolStack *stack);
static int  connector_Api_Opener(RtaConnection *conn);
static void connector_Api_Upcall_Read(PARCEventQueue *, PARCEventType, void *conn);
static int  connector_Api_Closer(RtaConnection *conn);
static int  connector_Api_Release(RtaProtocolStack *stack);
static void connector_Api_StateChange(RtaConnection *conn);

RtaComponentOperations api_ops =
{
    .init          = connector_Api_Init,
    .open          = connector_Api_Opener,
    .upcallRead    = connector_Api_Upcall_Read,
    .upcallEvent   = NULL,
    .downcallRead  = NULL,
    .downcallEvent = NULL,
    .close         = connector_Api_Closer,
    .release       = connector_Api_Release,
    .stateChange   = connector_Api_StateChange
};

// ========================

static int
connector_Api_Init(RtaProtocolStack *stack)
{
    // nothing to do here
    if (DEBUG_OUTPUT) {
        printf("%s init stack %p\n",
               __func__,
               (void *) stack);
    }
    return 0;
}

/*
 * Api_Open will put the RtaConnection as the callback parameter in the UpcallRead,
 * because its a per-connection descriptor.
 *
 * Returns 0 on success, -1 on error
 */
static int
connector_Api_Opener(RtaConnection *connection)
{
    RtaComponentStats *stats;
    RtaApiConnection *apiConnection = rtaApiConnection_Create(connection);

    rtaConnection_SetPrivateData(connection, API_CONNECTOR, apiConnection);

    stats = rtaConnection_GetStats(connection, API_CONNECTOR);
    assertNotNull(stats, "%s returned null stats\n", __func__);
    rtaComponentStats_Increment(stats, STATS_OPENS);

    rtaConnection_SetState(connection, CONN_OPEN);

    if (DEBUG_OUTPUT) {
        printf("%9" PRIu64 " %s opened transport_fd %d\n",
               rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(connection))),
               __func__,
               rtaConnection_GetTransportFd(connection));

        printf("%9" PRIu64 " %s open conn %p state %p\n",
               rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(connection))),
               __func__,
               (void *) connection,
               (void *) apiConnection);
    }

    return 0;
}

/*
 * Read a message from below in stack
 * Write a message up to the API
 */
static void
connector_Api_Upcall_Read(PARCEventQueue *eventBuffer, PARCEventType type, void *protocolStackVoid)
{
    TransportMessage *tm;

    assertNotNull(protocolStackVoid, "%s called with null ProtocolStack\n", __func__);

    while ((tm = rtaComponent_GetMessage(eventBuffer)) != NULL) {
        RtaConnection *conn = rtaConnection_GetFromTransport(tm);
        assertNotNull(conn, "got null connection from transport message\n");

        RtaComponentStats *stats = rtaConnection_GetStats(conn, API_CONNECTOR);
        assertNotNull(stats, "returned null stats\n");

        rtaComponentStats_Increment(stats, STATS_UPCALL_IN);

        RtaApiConnection *apiConnection = rtaConnection_GetPrivateData(conn, API_CONNECTOR);
        assertNotNull(apiConnection, "got null apiConnection\n");

        // If we are blocked, only pass control messages
        if (!rtaConnection_BlockedUp(conn) || transportMessage_IsControl(tm)) {
            if (!rtaApiConnection_SendToApi(apiConnection, tm, stats)) {
                // we failed to write.  Should a control message be re-queued? (case 879).
                // memory is freed at bottom of function
            }
        } else {
            // closed connection, just destroy the message
            if (DEBUG_OUTPUT) {
                printf("%9" PRIu64 " %s conn %p destroying transport message %p due to closed connection\n",
                       rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(conn))),
                       __func__,
                       (void *) conn,
                       (void *) tm);
            }
        }

        if (DEBUG_OUTPUT) {
            printf("%9" PRIu64 " %s conn %p total upcall reads in %" PRIu64 " out %" PRIu64 "\n",
                   rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(conn))),
                   __func__,
                   (void *) conn,
                   rtaComponentStats_Get(stats, STATS_UPCALL_IN),
                   rtaComponentStats_Get(stats, STATS_UPCALL_OUT));
        }

        // This is the end of life for the transport message.  If the inner TlvDictionary
        // was put in a CCNxMessage and sent up the stack, then we made another reference to it
        // so this destroy will not destroy that part.
        transportMessage_Destroy(&tm);
    }
}

/*
 * The higher layer should no longer be writing to this
 * socketpair, so we can drain it then close it.
 */
static int
connector_Api_Closer(RtaConnection *conn)
{
    RtaComponentStats *stats;
    RtaApiConnection *apiConnection = rtaConnection_GetPrivateData(conn, API_CONNECTOR);

    if (DEBUG_OUTPUT) {
        printf("%9" PRIu64 " %s starting close conn %p\n",
               rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(conn))),
               __func__,
               (void *) conn);
    }

    stats = rtaConnection_GetStats(conn, API_CONNECTOR);
    assertNotNull(stats, "%s returned null stats\n", __func__);
    rtaComponentStats_Increment(stats, STATS_CLOSES);

    // This will prevent any new data going in to queues for the connection
    // Existing messages will be destroyed
    rtaConnection_SetState(conn, CONN_CLOSED);

    rtaApiConnection_Destroy(&apiConnection);
    rtaConnection_SetPrivateData(conn, API_CONNECTOR, NULL);

    if (DEBUG_OUTPUT) {
        printf("%9" PRIu64 " %s close conn %p\n",
               rtaFramework_GetTicks(rtaProtocolStack_GetFramework(rtaConnection_GetStack(conn))),
               __func__,
               (void *) conn);
    }

    return 0;
}

static int
connector_Api_Release(RtaProtocolStack *stack)
{
    // nothing to do here, there's no ProtocolStack state
    if (DEBUG_OUTPUT) {
        printf("%s release stack %p\n",
               __func__,
               (void *) stack);
    }

    return 0;
}

/**
 * Respond to events for the connection
 *
 * Typcially, the forwarder connector will block and unblock the DOWN direction.  We need
 * to stop putting new data in the down directon if its blocked.
 *
 * The API connector (us) is generally the thing blocking the UP direction, so we don't need
 * to respond to those (our own) events.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *      ComponentOperations api_ops = {
 *          // [other settings]
 *          .stateChange  = connector_Api_StateChange
 *      };
 * }
 * @endcode
 */
static void
connector_Api_StateChange(RtaConnection *conn)
{
    RtaApiConnection *apiConnection = rtaConnection_GetPrivateData(conn, API_CONNECTOR);

    // we do not test the rtaConnection_BlockedUp() because we are the one setting those

    // If we are blocked in the DOWN direction, disable events on the read queue
    if (rtaConnection_BlockedDown(conn)) {
        rtaApiConnection_BlockDown(apiConnection);
    } else {
        rtaApiConnection_UnblockDown(apiConnection);
    }
}
