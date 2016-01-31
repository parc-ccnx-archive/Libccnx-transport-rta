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
 * @file rta_CommandDestroyProtocolStack.h
 * @brief Represents a command to destroy a protocol stack
 *
 * Used to construct an RtaCommand object that is passed to rtaTransport_PassCommand() or _rtaTransport_SendCommandToFramework()
 * to send a command from the API's thread of execution to the Transport's thread of execution.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_CommandDestroyProtocolStack_h
#define Libccnx_rta_CommandDestroyProtocolStack_h

struct rta_command_destroyprotocolstack;
typedef struct rta_command_destroyprotocolstack RtaCommandDestroyProtocolStack;

/**
 * Creates a DestroyProtocolStack command object
 *
 * Creates a DestroyProtocolStack command object used to signal the RTA framework to
 * destroy a protocol stack and all connections in it.
 *
 * @param [in] stackId The ID used to create the protocol stack.
 *
 * @return non-null An allocated object
 * @return null An error
 *
 * Example:
 * @code
 * void foo(RTATransport *transport)
 * {
 *     int stackId = nextStackIdNumber();
 *     CCNxStackConfig *config = ccnxStackConfig_Create();
 *
 *     RtaCommandCreateProtocolStack *createStack = rtaCommandCreateProtocolStack_Create(stackId, config);
 *     RtaCommand *command = rtaCommand_CreateCreateProtocolStack(createStack);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandCreateProtocolStack_Release(&createStack);
 *
 *     // ... do work ...
 *
 *     RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(stackId);
 *     command = rtaCommand_CreateDestroyProtocolStack(destroyStack);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandDestroyProtocolStack_Release(&destroyStack);
 *     ccnxStackConfig_Release(&config);
 * }
 * @endcode
 */
RtaCommandDestroyProtocolStack *rtaCommandDestroyProtocolStack_Create(int stackId);

/**
 * Increase the number of references to a `RtaCommandDestroyProtocolStack`.
 *
 * Note that new `RtaCommandDestroyProtocolStack` is not created,
 * only that the given `RtaCommandDestroyProtocolStack` reference count is incremented.
 * Discard the reference by invoking `rtaCommandDestroyProtocolStack_Release`.
 *
 * @param [in] destroyStack The RtaCommandDestroyProtocolStack to reference.
 *
 * @return non-null A reference to `destroyStack`.
 * @return null An error
 *
 * Example:
 * @code
 * {
 *     CCNxStackConfig *config = ccnxStackConfig_Create();
 *    RtaCommandDestroyProtocolStack *destroyStack = rtaCommandCreateProtocolStack_Create(stackId, config);
 *    RtaCommandDestroyProtocolStack *second = rtaCommandDestroyProtocolStack_Acquire(destroyStack);
 *
 *    // release order does not matter
 *    rtaCommandDestroyProtocolStack_Release(&destroyStack);
 *    rtaCommandDestroyProtocolStack_Release(&second);
 *    ccnxStackConfig_Release(&config);
 * }
 * @endcode
 */
RtaCommandDestroyProtocolStack *rtaCommandDestroyProtocolStack_Acquire(const RtaCommandDestroyProtocolStack *destroyStack);

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
 *     RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(stackId);
 *     RtaCommand *command = rtaCommand_CreateDestroyProtocolStack(destroyStack);
 *     _rtaTransport_SendCommandToFramework(transport, command);
 *     rtaCommand_Release(&command);
 *     rtaCommandDestroyProtocolStack(&destroyStack);
 * }
 * @endcode
 */
void rtaCommandDestroyProtocolStack_Release(RtaCommandDestroyProtocolStack **destroyStackPtr);

/**
 * Returns the Stack ID of the destroy stack command
 *
 * Returns the Stack ID parameter.
 *
 * @param [in] destroyStack An allocated RtaCommandDestroyProtocolStack
 *
 * @return integer The value passed to rtaCommandDestroyProtocolStack_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     RtaCommandDestroyProtocolStack *destroyStack = rtaCommandDestroyProtocolStack_Create(stackId);
 *     int testValue = rtaCommandDestroyProtocolStack_GetStackId(destroyStack);
 *     assertTrue(testValue == stackId, "Wrong value got %d expected %d", testValue, stackId);
 *     rtaCommandDestroyProtocolStack(&destroyStack);
 * }
 * @endcode
 */
int rtaCommandDestroyProtocolStack_GetStackId(const RtaCommandDestroyProtocolStack *destroyStack);
#endif // Libccnx_rta_CommandDestroyProtocolStack_h
