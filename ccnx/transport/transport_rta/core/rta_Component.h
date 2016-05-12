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
 * @file rta_Component.h
 * @brief <#Brief Description#>
 *
 * A Component is a functional block within a protocol stack.  It exists
 * between the API Connector (at the top) and the Forwarder Connector
 * (at the bottom).  All components have a similar interface.  The only
 * slight variation is that components betwen the Forwarder Connector
 * and the Codec deal in "wire" message formats, while components above
 * the connector deal with "parsed" (CCNxMessage) formats.
 *
 * To write a component, follow these procedures:
 * 1) add your component's name to components.h enum.  This is the
 *    symbolic name you will use for it in the code.  We'll call
 *    it PROTO_WIZ.
 * 2) Copy a skeleton, such as component_Verifier.h for your header.
 *    Let's call it component_Wizard.h.  Inside the header, you'll
 *    define the "operations" structure that's exported to the system.
 * @code{.c}
 **#ifndef Libccnx_component_wizard_h
 **#define Libccnx_component_wizard_h
 *
 * // Function structs for component variations
 * extern ComponentOperations proto_wizard_ops;
 *
 **#endif
 * @endcode
 *
 * 3) Copy a skeleton, like component_Verifier_Null.c, for your
 *    implementation.  Let's call it component_Wizard.c.  Inside
 *    you must:
 *    a) instantiate proto_wizard_ops:
 * @code{.c}
 * static int  component_Wizard_Init(ProtocolStack *stack);
 * static int  component_Wizard_Opener(RtaConnection *conn);
 * static void component_Wizard_Upcall_Read(PARCEventQueue *, void *conn);
 * static void component_Wizard_Downcall_Read(PARCEventQueue *, void *conn);
 * static int  component_Wizard_Closer(RtaConnection *conn);
 * static int  component_Wizard_Release(ProtocolStack *stack);
 *
 * ComponentOperations verify_null_ops = {
 *   component_Wizard_Init,
 *   component_Wizard_Opener,
 *   component_Wizard_Upcall_Read,
 *   NULL,
 *   component_Wizard_Downcall_Read,
 *   NULL,
 *   component_Wizard_Closer,
 *   component_Wizard_Release
 * };
 * @endcode
 *
 *       These define the interface your component exposes to the stack
 *       Init:  called once on stack creation
 *       Open:  called once per connection Open
 *       UpcallRead: Called when the "upward" buffer has something to read
 *       DowncallRead: Called when the "downward" buffer has something to read
 *       Closer: called once per connection Close
 *       Release: called on protocol stack destruction.
 *
 *       Optionally, you may include UpcallEvents and DowncallEvents, but
 *       in general those are not useful.
 *
 *       Any of the function pointers in the "ops" may be NULL.
 *
 *    b) Implement your Init.  If you need to create a stack-wide data structure
 *       to track state, you would do something like this, which allocates
 *       memory and sticks it away in component-specific storage in the stack.
 *       Notice that protocolStack_SetPrivateData takes our protocol's name
 *       PROTO_WIZ as a parameter.
 *
 * @code{.c}
 * static int
 * component_Wizard_Init(ProtocolStack *stack)
 * {
 *  struct mydata *data = mydata_Create();
 *  protocolStack_SetPrivateData(stack, PROTO_WIZ, data);
 *  return 0;
 * }
 * @endcode
 *
 *    c) Implement your Opener.  You will very likely want to keep per-connection
 *       state.  This follows a similar method to the Init, but in a connection.
 *       We squirl away the connection-specific data similarly to the stack-wide
 *       data.  In addition, it's good practice to fetch your component's Stats
 *       for the connection and increment the OPENS counter for a successful open.
 *
 * @code{.c}
 * static int
 * component_Wizard_Opener(RtaConnection *connection)
 * {
 *   ComponentStats *stats;
 *   struct myState *mystate;
 *
 *   parcMemory_AlocateAndClear(&mystate, sizeof(void *), sizeof(struct api_conn_state));
 *   rtaConnection_SetPrivateData(connection, PROTO_WIZ, mystate);
 *
 *   stats = rtaConnection_GetStats(connection, PROTO_WIZ);
 *   stats_Increment(stats, STATS_OPENS);
 *   return 0;
 * }
 * @endcode
 *
 *    d) Implement your Close and Release.  These perform the inverse
 *       of the Open and Init.  They should fetch your private data, if
 *       any, and free it:
 * @code{.c}
 * static int
 * component_Wizard_Closer(RtaConnection *conn)
 * {
 *   ComponentStats *stats   = rtaConnection_GetStats(conn, PROTO_WIZ);
 *   struct myState *mystate = rtaConnection_GetPrivateData(conn, PROTO_WIZ);
 *
 *   stats_Increment(stats, STATS_CLOSES);
 *   myState_Destroy(&mystate);
 *   return 0;
 * }
 *
 * static int
 * component_Wizard_Release(ProtocolStack *stack)
 * {
 *   ComponentStats *stats   = protocoLStack_GetStats(stack, PROTO_WIZ);
 *   struct myData *mydata = protocolStack_GetPrivateData(stack, PROTO_WIZ);
 *
 *   stats_Increment(stats, STATS_CLOSES);
 *   myData_Destroy(&mydata);
 *   return 0;
 * }
 * @endcode
 *
 *    d) Implement your Read handlers.  They are similar for the upcall
 *       and downcall handlers.  The main issue to be aware of is that
 *       you must *drain* the queue on each call.  The callback is edge
 *       triggered.
 *
 *       Below we show an example of the Upcall read callback, which means
 *       there is data from below travelling up the stack. Therefore, we
 *       retrieve the RTA_UP output queue to pass messages up the stack.
 *       The while() loop is what drains the queue.
 *
 *       Note also that "ptr" is a pointer to the ProtocolStack that owns
 *       connecition (what your Init was called with).  The Connection information
 *       rides inside the transport message, and is retrieved with a call
 *       to transportMessage_GetInfo().
 *
 * @code{.c}
 * static void
 * component_Wizard_Upcall_Read(PARCEventQueue *in, PARCEvent_EventType event, void *ptr)
 * {
 *   ProtocolStack *stack = (ProtocolStack *) ptr;
 *   PARCEventQueue *out = protocoStack_GetPutQueue(stack, PROTO_WIZ, RTA_UP);
 *   TransportMessage *tm;
 *
 *   while( (tm = rtaComponent_GetMessage(in)) != NULL )
 *   {
 *       RtaConnection  *conn  = transportMessage_GetInfo(tm);
 *       ComponentStats *stats = rtaConnection_GetStats(conn, PROTO_WIZ);
 *       CCNxMessage *msg      = TransportMessage_GetCcnxMessage(tm);
 *
 *       stats_Increment(stats, STATS_UPCALL_IN);
 *
 *       // do something with the CCNxMessage
 *
 *       if( rtaComponent_PutMessage(out, tm) )
 *           stats_Increment(stats, STATS_UPCALL_OUT);
 *   }
 * }
 * @endcode
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
/**
 */

#ifndef Libccnx_rta_component_h
#define Libccnx_rta_component_h

#include "components.h"
#include "rta_ComponentQueue.h"
#include "rta_ComponentStats.h"

/**
 * Init:         one time initialization on first instantiation (0 success, -1 failure)
 * Open:         Per connection open, returns valid descriptor or -1 on failure
 * upcallRead:   Callback when one or more messages are available
 * downcallRead: Callback when one or more messages are available.
 * xEvent:       Called for events on the queue
 * Close:        Per connection close
 * Release:      One time release of state when whole stack taken down
 * stateChagne:  Called when there is a state change related to the connection
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
typedef struct {
    int (*init)(RtaProtocolStack *stack);
    int (*open)(RtaConnection *conn);
    void (*upcallRead)(PARCEventQueue *queue, PARCEventType events, void *stack);
    void (*upcallEvent)(PARCEventQueue *queue, PARCEventQueueEventType events, void *stack);
    void (*downcallRead)(PARCEventQueue *queue, PARCEventType events, void *stack);
    void (*downcallEvent)(PARCEventQueue *queue, PARCEventQueueEventType events, void *stack);
    int (*close)(RtaConnection *conn);
    int (*release)(RtaProtocolStack *stack);
    void (*stateChange)(RtaConnection *conn);
} RtaComponentOperations;

extern PARCEventQueue *rtaComponent_GetOutputQueue(RtaConnection *conn,
                                                   RtaComponents component,
                                                   RtaDirection direction);

/**
 * Send a message between components.  The API connector and Forwarder connector
 * must set the connection information in the transport message with
 * rtaConnection_SetInTransport().
 *
 * returns 1 on success, 0 on failure
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
extern int rtaComponent_PutMessage(PARCEventQueue *queue, TransportMessage *tm);

/**
 * Fetch a message from the queue.  Will return NULL if no message
 * is available.
 *
 * As a side effect, it will drain message on a closed connection.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
extern TransportMessage *rtaComponent_GetMessage(PARCEventQueue *queue);
#endif
