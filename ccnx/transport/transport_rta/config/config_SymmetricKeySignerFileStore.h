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
 * @file config_SymmetricKeySignerFileStore.h
 * @brief Generates stack and connection configuration information
 *
 * Each component in the protocol stack must have a configuration element.
 * This module generates the configuration elements for the Symmetric Keystore.
 * The keystore is specific to a Connection, so there is no Protocol Stack configuration.
 *
 * @code
 * {
 *      // Configure a stack with {APIConnector,TLVCodec,MetisConnector}
 *      // The coded will use a symmetric keystore
 *
 *      stackConfig = ccnxStackConfig_Create();
 *      connConfig = ccnxConnectionConfig_Create();
 *
 *      apiConnector_ProtocolStackConfig(stackConfig);
 *      apiConnector_ConnectionConfig(connConfig);
 *      tlvCodec_ProtocolStackConfig(stackConfig);
 *      tlvCodec_ConnectionConfig(connConfig);
 *      symmetricKeySignerFileStore_ConnectionConfig(connConfig, "~/.ccnx/keystore.p12", "foobar password");
 *
 *      metisForwarder_ProtocolStackConfig(stackConfig);
 *      metisForwarder_ConnectionConfig(connConfig, metis_port);
 *
 *      CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connConfig);
 * }
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef Libccnx_config_SymmetricKeySignerFileStore_h
#define Libccnx_config_SymmetricKeySignerFileStore_h

#include <ccnx/transport/common/ccnx_TransportConfig.h>
#include <stdbool.h>

struct symmetrickeysigner_params {
    const char *filename;
    const char *password;
};

/**
 * Generates the configuration settings included in the Connection configuration
 *
 * Adds configuration elements to the `CCNxConnectionConfig`
 *
 * { "SIGNER" : "SymmetricFileStore",
 *   "SymmetricFileStore" : { "filename" : filename, password : password }
 * }
 *
 * @param [in] config A pointer to a valid CCNxConnectionConfig instance.
 *
 * @return non-null The modified `CCNxConnectionConfig`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxConnectionConfig *symmetricKeySignerFileStore_ConnectionConfig(CCNxConnectionConfig *config, const char *filename, const char *password);

/**
 * Returns the text string for this component
 *
 * Used as the text key to a JSON block.  You do not need to free it.
 *
 * @return non-null A text string unique to this component
 *
 */
const char *symmetricKeySignerFileStore_GetName(void);

/**
 * Look inside a JSON configuration and extract the Signer parameters
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [out] output The filename and password passed down the stack
 *
 * @return true Configuration item found and output set
 * @return false Configuration item not found, output not set
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool symmetricKeySignerFileStore_GetConnectionParams(PARCJSON *connectionJson, struct symmetrickeysigner_params *output);
#endif // Libccnx_config_SymmetricKeySignerFileStore_h
