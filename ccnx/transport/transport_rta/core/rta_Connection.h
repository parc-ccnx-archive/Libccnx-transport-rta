/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file rta_Connection.h
 * @brief <#Brief Description#>
 *
 * A connection embodies an API connection to the forwarder.  Multiple
 * connections are multiplexed over one stack.  A connection, however,
 * is largely independent of a particular stack.  All the RTA connections
 * are stored in RtaConnectionTable, which is managed by the Framework.
 *
 * A problem arises using queues between components, because there may
 * be messages in queue that cannot be free'd without slogging through
 * all the queues.
 *
 * Therefore, a connection tracks the number of messages in queue and
 * will not be freed until all messages in queue are flushed.
 *
 * A connection carries an "isopen" flag.  If it is false, no new
 * messages can go in to the connection.  Any message dequeued that
 * references a closed connection discarded.
 *
 * Once the connection reaches 0 messages in queue, if it is closed,
 * it is elegible for garbage collection.  componentServq will call
 * the _Destroy() method.  Destroy() only works if the refcount for
 * the connection is 0.  If the ProtocolStack still has a reference
 * to the connection, the connection will not be destroyed until
 * the protocol stack calls Destroy.
 *
 * A Connection may live longer than its protocol stack.  In the _Destroy,
 * it should not make reference to the protocol stack.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_Rta_Connection_h
#define Libccnx_Rta_Connection_h

#include <sys/queue.h>
#include <ccnx/transport/common/transport.h>
#include <ccnx/transport/transport_rta/core/components.h>
#include <ccnx/transport/transport_rta/core/rta_ProtocolStack.h>
#include <ccnx/transport/transport_rta/core/rta_ComponentStats.h>
#include <ccnx/transport/transport_rta/commands/rta_CommandOpenConnection.h>

#include <ccnx/api/notify/notify_Status.h>

struct rta_connection;
/**
 *
 * @see rtaConnection_Create
 */
typedef struct rta_connection RtaConnection;

typedef enum {
    CONN_OPEN,
    CONN_CLOSED,
    CONN_PAUSED
} RtaConnectionStateType;

/**
 * Create a connection and set the refcount to 1.  If the connection
 * pointer is stored by multiple entities, they should call
 * IncrementRefcount.  Calling _Destroy() decrements the refcount.
 *
 * transport_fd is our side of the data socketpair provided by rtaTransport.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *rtaConnection_Create(RtaProtocolStack *stack, const RtaCommandOpenConnection *cmdOpen);

/**
 * Get a reference counted copy
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *rtaConnection_Copy(RtaConnection *original);

/**
 * Destroys the object if this call decrements the refcount to 0.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaConnection_Destroy(RtaConnection **connPtr);

/**
 * Same as _Destroy, but for using in a TransportMessage Info.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaConnection_FreeFunc(void **voidPtr);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
RtaProtocolStack *rtaConnection_GetStack(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
RtaFramework *rtaConnection_GetFramework(const RtaConnection *connection);

/**
 *
 * Used to store per-connection state from Open.
 * Should be freed in Close, but you don't need to set it NULL.
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
void rtaConnection_SetPrivateData(RtaConnection *connection, RtaComponents component, void *private);

/**
 * Used to store per-connection state from Open
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
void *rtaConnection_GetPrivateData(RtaConnection *connection, RtaComponents component);

/**
 * Returns the connection state (open, paused, closed)
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnectionStateType rtaConnection_GetState(RtaConnection *connection);

/**
 * Sets the connection state
 *
 *   The API connector manages the connection state.  open means all messages
 *   may flow.  Paused means no new messages flow.  closed means all existing
 *   messages will be destroyed.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaConnection_SetState(RtaConnection *connection, RtaConnectionStateType state);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
RtaComponentStats *rtaConnection_GetStats(RtaConnection *connection, RtaComponents component);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
unsigned rtaConnection_IncrementMessagesInQueue(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
unsigned rtaConnection_DecrementMessagesInQueue(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
unsigned rtaConnection_MessagesInQueue(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
unsigned rtaConnection_GetConnectionId(const RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
int rtaConnection_GetStackId(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
int  rtaConnection_GetApiFd(RtaConnection *connection);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
int  rtaConnection_GetTransportFd(RtaConnection *connection);

/**
 * Creates a status message (see ccnx/api/notify) and sends it up or down the stack.
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
extern void rtaConnection_SendStatus(RtaConnection *connection,
                                     RtaComponents component,
                                     RtaDirection direction,
                                     NotifyStatusCode code,
                                     CCNxName *optionalName,
                                     const char *optionalMessage);

/**
 * Creates a status message (see ccnx/api/notify) and sends it up or down the stack.
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *rtaConnection_GetFromTransport(TransportMessage *tm);

/**
 * Creates a status message (see ccnx/api/notify) and sends it up or down the stack.
 *
 *   <#Discussion#>
 *
 * @param connection
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *rtaConnection_GetParameters(RtaConnection *connection);

/**
 * Is the connection blocked in the down direction?
 *
 * Will return true if the connection is not open (DOWN or PAUSED state) or if the
 * given direction is blocked.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return true Connection blocked, will not accept any more packets in down direction
 * @return false Connection not blocked in down direction
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool rtaConnection_BlockedDown(const RtaConnection *connection);

/**
 * Is the connection blocked in the up direction?
 *
 * Will return true if the connection is not open (DOWN or PAUSED state) or if the
 * given direction is blocked.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return true Connection blocked, will not accept any more packets in up direction
 * @return false Connection not blocked in up direction
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool rtaConnection_BlockedUp(const RtaConnection *connection);

void rtaConnection_SetBlockedDown(RtaConnection *connection);
void rtaConnection_ClearBlockedDown(RtaConnection *connection);

void rtaConnection_SetBlockedUp(RtaConnection *connection);
void rtaConnection_ClearBlockedUp(RtaConnection *connection);
#endif
