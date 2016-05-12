/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file rta_Transport.h
 * @brief Global state for the entire transport
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <stdint.h>
#include <ccnx/transport/common/transport.h>

/**
 * rtaTransport executes in the API's thread.  It glues the bottom half of
 * the Transport API to the RTA transport.  It owns and manages a worker thread
 * in which the event schduler executes.
 *
 * rtaTransport is thread safe.  You may have multiple threads using the same
 * transport context.
 *
 * Inside the worker thread, the event scheduler executes without locks.  Therefore we need
 * to message pass to it and have it execute our commands in a managed callback.
 * This is done by passing commands (JSON) over a socket pair.
 *
 * Inside the worker thread, rta_Framework provides service utilities to components
 * and connectors.  It also manages the command socket.
 *
 * When an API calls <code>int rtaTransport_Open(CCNxJSON *params)</code>, rtaTransport
 * will create a socket pair and give one back to the api (api_fd) and send one to
 * rtaFramework (transport_fd).
 *
 * The socket commands are (in JSON):
 *
 * PARAMS := existing SYSTEM and USER JSON objects, i.e.:
 * { "SYSTEM" : {...}, "USER" : {...} }
 *
 * { "RTA" : { "CREATE STACK" : stack_id, PARAMS }
 * { "RTA" : { "OPEN" : [stack_id, api_fd, transport_fd], PARAMS } }
 * { "RTA" : { "CLOSE": transport_fd } }
 * { "RTA" : { "DESTROY STACK": stack_id } }
 * { "RTA" : { "SHUTDOWN" }
 *
 * See rta_Commands.h for an implementation of this.
 */
#ifndef Libccnx_rta_Transport_h
#define Libccnx_rta_Transport_h

#include <ccnx/transport/common/transport.h>
#include <ccnx/transport/transport_rta/commands/rta_Command.h>

/**
 * Transport Ready To Assemble context
 *
 */
struct rta_transport;
typedef struct rta_transport RTATransport;

/**
 * Structure of function points to operate on Transport RTA
 *
 */
extern const struct transport_operations rta_ops;

/**
 * Create the transport.  No locks here, as rtaFramework_Create and rtaFramework_Start
 * are thread-safe functions and we dont maintain any data.
 *
 */
RTATransport *rtaTransport_Create(void);

int rtaTransport_Destroy(RTATransport **ctxPtr);

int rtaTransport_Open(RTATransport *ctx, CCNxTransportConfig *transportConfig);

/**
 * Send a CCNxMetaMessage on the outbound direction of the stack.
 *
 * @param [in] transport A pointer to a valid RTATransport instance.
 * @param [in] queueId The identifier of the asynchronous queue between the top and bottom halves of the stack.
 * @param [in] message A pointer to a valid CCNxMetaMessage instance.
 *
 * @return true The send was successful
 * @return false The send was not successful
 */
//bool rtaTransport_Send(RTATransport *transport, int queueId, const CCNxMetaMessage *message, const struct timeval *restrict timeout);
bool rtaTransport_Send(RTATransport *transport, int queueId, const CCNxMetaMessage *message, const uint64_t *microSeconds);

TransportIOStatus rtaTransport_Recv(RTATransport *transport, const int queueId, CCNxMetaMessage **msgPtr, const uint64_t *microSeconds);

int rtaTransport_Close(RTATransport *transport, int desc);

int rtaTransport_PassCommand(RTATransport *transport, const RtaCommand *rtacommand);

#endif // Libccnx_rta_Transport_h
