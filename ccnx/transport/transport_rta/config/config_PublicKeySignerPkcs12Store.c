/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
//
//  config_PublicKeySignerPkcs12Store.c
//  Libccnx
//
//  Created by Mosko, Marc <Marc.Mosko@parc.com> on 10/23/13.
//
//
#include <config.h>
#include <LongBow/runtime.h>

#include <stdio.h>

#include <parc/security/parc_Identity.h>
#include "config_PublicKeySignerPkcs12Store.h"

#include <ccnx/transport/transport_rta/core/components.h>

static const char name[] = "publicKeySignerPkcs12Store";

static const char param_KEYSTORE_NAME[] = "KEYSTORE_NAME";
static const char param_KEYSTORE_PASSWD[] = "KEYSTORE_PASSWD";
static const char param_SIGNER[] = "SIGNER";

/**
 * Generates:
 *
 * { "SIGNER" : "publicKeySignerPkcs12Store",
 * "publicKeySignerPkcs12Store" : { "filename" : filename, "password" : password }
 * }
 */
CCNxConnectionConfig *
configPublicKeySignerPkcs12Store_SetIdentity(CCNxConnectionConfig *connConfig, const PARCIdentity *identity)
{
    return publicKeySignerPkcs12Store_ConnectionConfig(connConfig,
                                                       parcIdentity_GetFileName(identity),
                                                       parcIdentity_GetPassWord(identity));
}

CCNxConnectionConfig *
publicKeySignerPkcs12Store_ConnectionConfig(CCNxConnectionConfig *connConfig, const char *filename, const char *password)
{
    assertNotNull(connConfig, "Parameter connConfig must be non-null");
    assertNotNull(filename, "Parameter filename must be non-null");
    assertNotNull(password, "Parameter password must be non-null");

    PARCJSONValue *signerNameJson = parcJSONValue_CreateFromCString((char *) publicKeySignerPkcs12Store_GetName());
    ccnxConnectionConfig_Add(connConfig, param_SIGNER, signerNameJson);
    parcJSONValue_Release(&signerNameJson);

    PARCJSON *keystoreJson = parcJSON_Create();
    parcJSON_AddString(keystoreJson, param_KEYSTORE_NAME, filename);
    parcJSON_AddString(keystoreJson, param_KEYSTORE_PASSWD, password);

    PARCJSONValue *value = parcJSONValue_CreateFromJSON(keystoreJson);
    parcJSON_Release(&keystoreJson);
    CCNxConnectionConfig *result = ccnxConnectionConfig_Add(connConfig, publicKeySignerPkcs12Store_GetName(), value);
    parcJSONValue_Release(&value);
    return result;
}

const char *
publicKeySignerPkcs12Store_GetName()
{
    return name;
}

/**
 * If successful, return true and fill in the parameters in the output argument
 */
bool
publicKeySignerPkcs12Store_GetConnectionParams(PARCJSON *connectionJson, struct publickeysigner_params *output)
{
    assertNotNull(connectionJson, "Parameter connectionJson must be non-null");
    assertNotNull(output, "Parameter output must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(connectionJson, publicKeySignerPkcs12Store_GetName());
    assertNotNull(value, "JSON key %s missing", publicKeySignerPkcs12Store_GetName());
    PARCJSON *keystoreJson = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(keystoreJson, param_KEYSTORE_NAME);
    assertNotNull(value, "JSON key %s missing inside %s", param_KEYSTORE_NAME, publicKeySignerPkcs12Store_GetName());
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    output->filename = parcBuffer_Overlay(sBuf, 0);

    value = parcJSON_GetValueByName(keystoreJson, param_KEYSTORE_PASSWD);
    assertNotNull(value, "JSON key %s missing inside %s", param_KEYSTORE_PASSWD, publicKeySignerPkcs12Store_GetName());
    sBuf = parcJSONValue_GetString(value);
    output->password = parcBuffer_Overlay(sBuf, 0);


    return true;
}
