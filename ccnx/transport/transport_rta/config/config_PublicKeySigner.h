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
 * @file config_PublicKeySigner.h
 * @brief Generates stack and connection configuration information
 *
 * Each component in the protocol stack must have a configuration element.
 * This module generates the configuration elements for the PKCS12 Signer.
 *
 * The signer only as a Connection configuration.
 *
 * @code
 * {
 *      // Configure a stack with {APIConnector,TLVCodec,PKCS12Signer,MetisConnector}
 *
 *      stackConfig = ccnxStackConfig_Create();
 *      connConfig = ccnxConnectionConfig_Create();
 *
 *      apiConnector_ProtocolStackConfig(stackConfig);
 *      apiConnector_ConnectionConfig(connConfig);
 *      tlvCodec_ProtocolStackConfig(stackConfig);
 *      tlvCodec_ConnectionConfig(connConfig);
 *
 *      publicKeySigner_ConnectionConfig(connConfig, "~/.ccnx/keystore.p12", "foobar password");
 *
 *      metisForwarder_ProtocolStackConfig(stackConfig);
 *      metisForwarder_ConnectionConfig(connConfig, metisForwarder_GetDefaultPort());
 *
 *      CCNxTransportConfig *config = ccnxTransportConfig_Create(stackConfig, connConfig);
 * }
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_config_PublicKeySigner_h
#define Libccnx_config_PublicKeySigner_h
#include <stdbool.h>

#include <parc/security/parc_Identity.h>

#include <ccnx/transport/common/ccnx_TransportConfig.h>

struct publickeysigner_params {
    const char *filename;
    const char *password;
};

/**
 * Generates the configuration settings included in the Connection configuration
 *
 * Adds configuration elements to the `CCNxConnectionConfig`
 *
 * { "SIGNER" : "publicKeySigner",
 *   "publicKeySigner" : { "filename" : filename, "password" : password }
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
CCNxConnectionConfig *publicKeySigner_ConnectionConfig(CCNxConnectionConfig *config, const char *filename, const char *password);

/**
 * Generates the configuration settings included in the CCNxConnectionConfig for the identity of the configuration 'owner'
 *
 * Adds configuration elements to the `CCNxConnectionConfig`
 *
 * { "SIGNER" : "publicKeySigner",
 *   "publicKeySigner" : { "filename" : filename, "password" : password }
 * }
 *
 * @param [in] connConfig The pointer to a valid CCNxConnectionConfig instance.
 * @param [in] identity A pointer to a valid PARCIdentity instance.
 *
 * @return non-null The modified `CCNxConnectionConfig`
 */
CCNxConnectionConfig *configPublicKeySigner_SetIdentity(CCNxConnectionConfig *connConfig, const PARCIdentity *identity);

/**
 * Returns the text string for this component
 *
 * Used as the text key to a JSON block.  You do not need to free it.
 *
 * @return non-null A text string unique to this component
 *
 */
const char *publicKeySigner_GetName(void);

/**
 * If successful, return true and fill in the parameters in the output argument
 *
 * Parses the JSON created by publicKeySigner_ConnectionConfig() and fills in the
 * output parameter.  The output parameter must be allocated by the caller.
 */
bool publicKeySigner_GetConnectionParams(PARCJSON *connectionJson, struct publickeysigner_params *output);
#endif // Libccnx_config_PublicKeySigner_h
