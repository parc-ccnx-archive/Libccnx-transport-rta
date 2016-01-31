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
 * @file config_ProtocolStack.h
 * @brief Generates stack and connection configuration information
 *
 * Each component in the protocol stack must have a configuration element.
 * This module generates the configuration elements for the ProtocolStack.
 *
 * The ProtocolStack configuration is a list of key names for the components
 * in the stack.  It is an in-order list of the components to configure in the
 * stack.
 *
 * @code
 * {
 *      // Configure a stack with {APIConnector,TLVCodec,MetisConnector}
 *
 *      stackConfig = ccnxStackConfig_Create();
 *      connConfig = ccnxConnectionConfig_Create();
 *
 *      apiConnector_ProtocolStackConfig(stackConfig);
 *      apiConnector_ConnectionConfig(connConfig);
 *      tlvCodec_ProtocolStackConfig(stackConfig);
 *      tlvCodec_ConnectionConfig(connConfig);
 *      inMemoryVerifier_ConnectionConfig(connConfig);
 *      metisForwarder_ProtocolStackConfig(stackConfig);
 *      metisForwarder_ConnectionConfig(connConfig, metisForwarder_GetDefaultPort());
 *
 *      protocolStack_ComponentsConfigArgs(stackConfig, apiConnector_Name(), tlvCodec_Name(), metisForwarder_Name(), NULL);
 *
 *      CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connConfig);
 * }
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef Libccnx_config_ProtocolStack_h
#define Libccnx_config_ProtocolStack_h

#include <ccnx/transport/common/ccnx_TransportConfig.h>
#include <parc/algol/parc_ArrayList.h>

/**
 * Generates the configuration settings included in the Protocol Stack configuration
 *
 * Adds configuration elements to the Protocol Stack configuration
 *
 * { "COMPONENTS" : [ name1, name2, ... ] }
 *
 * The ProtocolStack function adds a configuration element that enumerates each component
 * that will be in the protocol stack, in order.  These names must match the names
 * used by each component in its own particular configuration.
 *
 * @param [in] stackConfig The protocl stack configuration to update
 *
 * @return non-null The updated protocol stack configuration
 *
 * Example:
 * @code
 * {
 *      protocolStack_ComponentsConfigArgs(stackConfig, apiConnector_Name(), tlvCodec_Name(), metisForwarder_Name(), NULL);
 * }
 * @endcode
 */
CCNxStackConfig *protocolStack_ComponentsConfigArgs(CCNxStackConfig *stackConfig, ...);

/**
 * Generates the configuration settings included in the Protocol Stack configuration
 *
 * Adds configuration elements to the Protocol Stack configuration
 *
 * { "COMPONENTS" : [ name1, name2, ... ] }
 *
 * The ProtocolStack function adds a configuration element that enumerates each component
 * that will be in the protocol stack, in order.  These names must match the names
 * used by each component in its own particular configuration.
 *
 * @param [in] stackConfig The protocl stack configuration to update
 *
 * @return non-null The updated protocol stack configuration
 *
 * Example:
 * @code
 * @endcode
 */
CCNxStackConfig *protocolStack_ComponentsConfigArrayList(CCNxStackConfig *stackConfig, const PARCArrayList *listOfComponentNames);

/**
 * Returns the text string for this component
 *
 * Used as the text key to a JSON block.  You do not need to free it.
 *
 * @return non-null A text string unique to this component
 *
 */
const char *protocolStack_GetName(void);

/**
 * Parse the protocol stack json to extract an array list of the component names
 */
PARCArrayList *protocolStack_GetComponentNameArray(PARCJSON *stackJson);
#endif // Libccnx_config_ProtocolStack_h
