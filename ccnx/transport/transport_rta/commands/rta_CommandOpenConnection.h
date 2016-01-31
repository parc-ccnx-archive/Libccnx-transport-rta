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
 * @file rta_CommandOpenConnection.h
 * @brief Represents a command to open a connection
 *
 * Used to construct an RtaCommand object that is passed to rtaTransport_PassCommand() or _rtaTransport_SendCommandToFramework()
 * to send a command from the API's thread of execution to the Transport's thread of execution.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_CommandOpenConnection_h
#define Libccnx_rta_CommandOpenConnection_h


struct rta_command_openconnection;
typedef struct rta_command_openconnection RtaCommandOpenConnection;

/**
 * Creates a OpenConnection command object
 *
 * Creates a OpenConnection command object used to signal the RTA framework to
 * open a specified connection.  The user passes in the two sides of a socket pair
 * plus the JSON representation of the connection.
 *
 * The RTA transport API connector will use the transportNotifierFd side of the socket pair.
 *
 * The caller must know the protocol stack handle stackId to specify which stack to
 * associate the connection with.
 *
 * @param [in] stackId The protocol stack handle to use for the connection.
 * @param [in] apiNotifierFd A descriptor used to notify the API that there's data to read from the transport.
 * @param [in] transportNotifierFd A descriptor used to notify the Transport there's data to read from the API.
 * @param [in] config The stack/connection config.
 *
 * @return non-null An allocated object
 * @return null An error
 *
 * Example:
 * @code
 * void foo(RTATransport *transport)
 * {
 *     #define API_SIDE 0
 *     #define TRANSPORT_SIDE 1
 *
 *     int pair[2];
 *     socketpair(AF_LOCAL, SOCK_STREAM, 0, pair);
 *     PARCJSON *config = createConnectionConfig();
 *
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(6, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     RtaCommand *command = rtaCommand_CreateOpenConnection(openCommand);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandOpenConnection_Release(&openCommand);
 *
 *     // ... do work ...
 *     RtaCommandCloseConnection *closeCommand = rtaCommandCloseConnection_Create(pair[API_SIDE]);
 *     RtaCommand *command = rtaCommand_CreateCloseConnection(openCommand);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandCloseConnection_Release(&closeCommand);
 * }
 * @endcode
 */
RtaCommandOpenConnection *rtaCommandOpenConnection_Create(int stackId, int apiNotifierFd, int transportNotifierFd, const PARCJSON *config);

/**
 * Increase the number of references to a `RtaCommandOpenConnection`.
 *
 * Note that new `RtaCommandOpenConnection` is not created,
 * only that the given `RtaCommandOpenConnection` reference count is incremented.
 * Discard the reference by invoking `rtaCommandOpenConnection_Release`.
 *
 * @param [in] openConnection The RtaCommandDestroyProtocolStack to reference.
 *
 * @return non-null A reference to `openConnection`.
 * @return null An error
 *
 * Example:
 * @code
 * {
 *    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(6, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *    RtaCommandOpenConnection *second = rtaCommandOpenConnection_Acquire(openConnection);
 *
 *    // release order does not matter
 *    rtaCommandOpenConnection_Release(&openConnection);
 *    rtaCommandOpenConnection_Release(&second);
 * }
 * @endcode
 */
RtaCommandOpenConnection *rtaCommandOpenConnection_Acquire(const RtaCommandOpenConnection *openConnection);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] closePtr A pointer to the object to release, will return NULL'd.
 *
 * Example:
 * @code
 * {
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(6, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     RtaCommand *command = rtaCommand_CreateOpenConnection(openCommand);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandOpenConnection_Release(&openCommand);
 * }
 * @endcode
 */
void rtaCommandOpenConnection_Release(RtaCommandOpenConnection **openPtr);

/**
 * Returns the Stack ID of the open command
 *
 * Returns the Stack ID parameter.
 *
 * @param [in] openConnection An allocated RtaCommandOpenConnection
 *
 * @return integer The value passed to rtaCommandOpenConnection_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(apiSocket, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     int testValue = rtaCommandOpenConnection_GetStackId(openCommand);
 *     assertTrue(testValue == stackId, "Wrong value got %d expected %d", testValue, stackId);
 *     rtaCommandOpenConnection_Release(&openCommand);
 * }
 * @endcode
 */
int rtaCommandOpenConnection_GetStackId(const RtaCommandOpenConnection *openConnection);

/**
 * Returns the API descriptor of the open command
 *
 * Returns the apiNotifierFd parameter.  The API descriptor is the side read/written by the API.
 *
 * @param [in] openConnection An allocated RtaCommandOpenConnection
 *
 * @return integer The value passed to rtaCommandOpenConnection_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(apiSocket, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     int testValue = rtaCommandOpenConnection_GetApiNotifierFd(openCommand);
 *     assertTrue(testValue == pair[API_SIDE], "Wrong value got %d expected %d", testValue, pair[API_SIDE]);
 *     rtaCommandOpenConnection_Release(&openCommand);
 * }
 * @endcode
 */
int rtaCommandOpenConnection_GetApiNotifierFd(const RtaCommandOpenConnection *openConnection);

/**
 * Returns the Transport descriptor of the open command
 *
 * Returns the transportNotifierFd parameter.  The transport descriptor is the side read/written by the Transport.
 *
 * @param [in] openConnection An allocated RtaCommandOpenConnection
 *
 * @return integer The value passed to rtaCommandOpenConnection_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(apiSocket, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     int testValue = rtaCommandOpenConnection_GetTransportNotifierFd(openCommand);
 *     assertTrue(testValue == pair[TRANSPORT_SIDE], "Wrong value got %d expected %d", testValue, pair[TRANSPORT_SIDE]);
 *     rtaCommandOpenConnection_Release(&openCommand);
 * }
 * @endcode
 */
int rtaCommandOpenConnection_GetTransportNotifierFd(const RtaCommandOpenConnection *openConnection);

/**
 * Returns the PARCJSON stack configuration of the create stack command
 *
 * Returns the JSON representation of the stack configuration.
 *
 * @param [in] createStack An allocated RtaCommandCreateProtocolStack
 *
 * @return The value passed to rtaCommandCreateProtocolStack_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     PARCJSON *config = createConnectionConfiguration();
 *     RtaCommandOpenConnection *openCommand = rtaCommandOpenConnection_Create(apiSocket, pair[API_SIDE], pair[TRANSPORT_SIDE], config);
 *     PARCJSON *testValue = rtaCommandOpenConnection_GetConfig(openCommand);
 *     assertTrue(ccnxJson_Equals(config, testValue), "Wrong value");
 *     rtaCommandOpenConnection_Release(&openCommand);
 *     parcJSON_Release(&config);
 * }
 * @endcode
 */
PARCJSON *rtaCommandOpenConnection_GetConfig(const RtaCommandOpenConnection *openConnection);
#endif // Libccnx_rta_CommandOpenConnection_h
