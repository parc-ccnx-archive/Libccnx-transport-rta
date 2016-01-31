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

#include <LongBow/runtime.h>

#include <parc/algol/parc_EventBuffer.h>

#include <ccnx/transport/common/transport_Message.h>
#include <ccnx/transport/transport_rta/rta_Transport.h>
#include <ccnx/transport/transport_rta/core/rta_ProtocolStack.h>
#include <ccnx/transport/transport_rta/core/rta_Connection.h>
#include <ccnx/transport/transport_rta/core/rta_Component.h>


#ifndef DEBUG_OUTPUT
#define DEBUG_OUTPUT 0
#endif

PARCEventQueue *
rtaComponent_GetOutputQueue(RtaConnection *conn,
                            RtaComponents component,
                            RtaDirection direction)
{
    RtaProtocolStack *stack;

    assertNotNull(conn, "called with null connection\n");

    stack = rtaConnection_GetStack(conn);
    assertNotNull(stack, "resolved null stack\n");

    return rtaProtocolStack_GetPutQueue(stack, component, direction);
}

int
rtaComponent_PutMessage(PARCEventQueue *queue, TransportMessage *tm)
{
    RtaConnection *conn = rtaConnection_GetFromTransport(tm);
    assertNotNull(conn, "Got null connection from transport message\n");

    if (rtaConnection_GetState(conn) != CONN_CLOSED) {
        PARCEventBuffer *out = parcEventBuffer_GetQueueBufferOutput(queue);
        int res;

        rtaConnection_IncrementMessagesInQueue(conn);

        if (DEBUG_OUTPUT) {
            printf("%s  queue %-12s tm %p\n",
                   __func__,
                   rtaProtocolStack_GetQueueName(rtaConnection_GetStack(conn), queue),
                   (void *) tm);
        }

        res = parcEventBuffer_Append(out, (void *)&tm, sizeof(&tm));
        assertTrue(res == 0, "%s parcEventBuffer_Append returned error\n", __func__);
        parcEventBuffer_Destroy(&out);
        return 1;
    } else {
        // should increment a drop counter (case 908)
        transportMessage_Destroy(&tm);

        return 0;
    }
}

TransportMessage *
rtaComponent_GetMessage(PARCEventQueue *queue)
{
    PARCEventBuffer *in = parcEventBuffer_GetQueueBufferInput(queue);

    while (parcEventBuffer_GetLength(in) >= sizeof(TransportMessage *)) {
        ssize_t len;
        TransportMessage *tm;
        RtaConnection *conn;

        len = parcEventBuffer_Read(in, (void *)&tm, sizeof(&tm));

        assertTrue(len == sizeof(TransportMessage *),
                   "parcEventBuffer_Read returned error");

        // Is the transport message for an open connection?
        conn = rtaConnection_GetFromTransport(tm);
        assertNotNull(conn, "%s GetInfo returnd null connection\n", __func__);

        if (DEBUG_OUTPUT) {
            printf("%s queue %-12s tm %p\n",
                   __func__,
                   rtaProtocolStack_GetQueueName(rtaConnection_GetStack(conn), queue),
                   (void *) tm);
        }

        (void) rtaConnection_DecrementMessagesInQueue(conn);

        if (rtaConnection_GetState(conn) != CONN_CLOSED) {
            parcEventBuffer_Destroy(&in);
            return tm;
        }

        // it's a closed connection

        if (DEBUG_OUTPUT) {
            printf("%s clearing connection %p reference in transport\n",
                   __func__, (void *) conn);
        }

        // should increment a drop counter (case 908)
        transportMessage_Destroy(&tm);
    }

    parcEventBuffer_Destroy(&in);
    return NULL;
}
