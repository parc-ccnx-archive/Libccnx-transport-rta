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
 * @file ccnx_TransportConfig.h
 * @brief The Transport Configuration information.
 *
 * The API composes the stack and connection parameters using these functions.
 * The examples below are for the RTA Transport.
 *
 * <code>
 * CCNxStackConfig *stackConfig = ccnxStackConfig_Create();
 * ccnxStackConfig_AppendComponents(stackConfig, { RtaComponentNames[API_CONNECTOR], RtaComponentNames[FC_VEGAS],
 * RtaComponentNames[CODEC_CCNB], RtaComponentNames[FWD_CCND] } );
 *
 * ccnxStackConfig_AppendApiConnector(stackConfig);
 * ccnxStackConfig_AppendVegasFlowController(stackConfig);
 * ccnxStackConfig_AppendCcndCodec(stackConfig);
 * ccnxStackConfig_AppendCcndForwarder(stackConfig);
 *
 * CCNxConnectionConfig *connConfig = ccnxConnectionConfig_Create();
 * ccnxConnectionConfig_publicKeySignerPkcs12Store(connConfig, "/Users/mmosko/keystore.p12", "123abc");
 * ccnxConnectionConfig_InMemoryVerifier(connConfig);
 *
 *
 * RtaCommand *cmdCreateStack = rtaCommand_CreateStack( (CommandCreateStack) { .stack_id = 7, .params = ccnxStackConfig_GetJson(stackConfig) } );
 * rtaCommand_write(cmdCreateStack, command_fd);
 * ccnxStackConfig_Release(&stackConfig);
 *
 * RtaCommand *cmdOpen = rtaCommand_Open( (CommandOpen) { .stack_id = 7, .api_fd = 12, .transport_fd = 13, .params = connecitonConfig_GetJson(connConfig) } );
 * rtaCommand_write(cmdCreateStack, command_fd);
 * ccnxConnectionConfig_Destroy(&connConfig);
 * </code>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef Libccnx_transport_Configuration_h
#define Libccnx_transport_Configuration_h

#include <stdarg.h>

#include <ccnx/transport/common/ccnx_StackConfig.h>
#include <ccnx/transport/common/ccnx_ConnectionConfig.h>

struct transport_config;
typedef struct transport_config CCNxTransportConfig;

/**
 * Create a `CCNxTransportConfig` instance.
 *
 * The instance must be populated with configuration information before it can be used.
 *
 * @param [in] stackConfig A pointer to a valid `CCNxStackConfig` instance.
 * @param [in] connectionConfig A pointer to a valid `CCNxConnectionConfig` instance.
 * @return NULL An error occurred.
 * @return non-NULL A valid `CCNxTransportConfig` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *
 *     ccnxTransportConfig_Destroy(&config);
 * @endcode
 */
CCNxTransportConfig *ccnxTransportConfig_Create(CCNxStackConfig *stackConfig, CCNxConnectionConfig *connectionConfig);

#ifdef CCNxTransport_DISABLE_VALIDATION
#  define ccnxTransportConfig_OptionalAssertValid(_instance_)
#else
#  define ccnxTransportConfig_OptionalAssertValid(_instance_) ccnxTransportConfig_AssertValid(_instance_)
#endif
/**
 * Assert that an instance of `CCNxTransportConfig` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] config A pointer to a `CCNxTransportConfig` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *     ccnxTransportConfig_AssertValid(config);
 *     ccnxTransportConfig_Destroy(&config);
 * }
 * @endcode
 * @see ccnxTransportConfig_IsValid
 */
void ccnxTransportConfig_AssertValid(const CCNxTransportConfig *config);

/**
 * Destroy previously created `CCNxTransportConfig` instance.
 *
 * @param [in] configPtr A pointer to a pointer to a valid `CCNxTransportConfig` instance that will be set to zero upon return.
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *
 *     ccnxTransportConfig_Destroy(&config);
 * }
 * @endcode
 */
void ccnxTransportConfig_Destroy(CCNxTransportConfig **configPtr);

/**
 * Get the `CCNxStackConfig` instance in the given `CCNxTransportConfig`
 *
 * @param [in] config A pointer to a valid `CCNxTransportConfig` instance.
 *
 * @return A pointer to the `CCNxStackConfig` instance in the given `CCNxTransportConfig`
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *
 *     CCNxStackConfig *stack = ccnxTransportConfig_GetStackConfig(config);
 *
 *     ccnxTransportConfig_Destroy(&config);
 * }
 * @endcode
 */
CCNxStackConfig *ccnxTransportConfig_GetStackConfig(const CCNxTransportConfig *config);

/**
 * Get the `CCNxConnectionConfig` instance in the given `CCNxTransportConfig`
 *
 * @param [in] config A pointer to a valid `CCNxTransportConfig` instance.
 *
 * @return A pointer to the `CCNxConnectionConfig` instance in the given `CCNxTransportConfig`
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *
 *     CCNxConnectionConfig *connection = ccnxTransportConfig_GetConnectionConfig(config);
 *
 *     ccnxTransportConfig_Destroy(&config);
 * }
 * @endcode
 */
CCNxConnectionConfig *ccnxTransportConfig_GetConnectionConfig(const CCNxTransportConfig *config);

/**
 * Determine if an instance of `CCNxTransportConfig` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] config A pointer to a `CCNxTransportConfig` instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connectionConfig);
 *     ccnxTransportConfig_IsValid(config);
 *     ccnxTransportConfig_Destroy(&config);
 * }
 * @endcode
 * @see ccnxTransportConfig_AssertValid
 */
bool ccnxTransportConfig_IsValid(const CCNxTransportConfig *config);
/**
 * Determine if two `CCNxTransportConfig` instances are equal.
 *
 * The following equivalence relations on non-null `CCNxTransportConfig` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `ccnxTransportConfig_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `ccnxTransportConfig_Equals(x, y)` must return true if and only if
 *        `ccnxTransportConfig_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `ccnxTransportConfig_Equals(x, y)` returns true and
 *        `ccnxTransportConfig_Equals(y, z)` returns true,
 *        then `ccnxTransportConfig_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `ccnxTransportConfig_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `ccnxTransportConfig_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid CCNxTransportConfig instance.
 * @param [in] y A pointer to a valid CCNxTransportConfig instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     CCNxTransportConfig *a = ccnxTransportConfig_Create();
 *     CCNxTransportConfig *b = ccnxTransportConfig_Create();
 *
 *     if (ccnxTransportConfig_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     ccnxTransportConfig_Release(&a);
 *     ccnxTransportConfig_Release(&b);
 * }
 * @endcode
 * @see ccnxTransportConfig_HashCode
 */
bool ccnxTransportConfig_Equals(const CCNxTransportConfig *x, const CCNxTransportConfig *y);

/**
 * Make a copy of the given TransportConfig.  The original and copy
 * must both be destroyed.
 */
CCNxTransportConfig *ccnxTransportConfig_Copy(const CCNxTransportConfig *original);
#endif // Libccnx_transport_Configuration_h
