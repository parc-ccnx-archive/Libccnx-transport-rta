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
//  config_Signer.c
//  Libccnx
//
//  Created by Mosko, Marc <Marc.Mosko@parc.com> on 10/23/13.
//
//

#include <config.h>
#include <stdio.h>
#include <string.h>
#include "config_Signer.h"
#include "config_PublicKeySigner.h"
#include "config_SymmetricKeySigner.h"

#include <ccnx/transport/transport_rta/core/components.h>

#include <LongBow/runtime.h>

static const char param_SIGNER[] = "SIGNER";

const char *
signer_GetName()
{
    return param_SIGNER;
}

/**
 * Determine which signer is configured
 */
SignerType
signer_GetImplementationType(PARCJSON *connectionJson)
{
    assertNotNull(connectionJson, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(connectionJson, signer_GetName());
    assertNotNull(value, "signer must be specified in the connection configuration");
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    const char *signer_name = parcBuffer_Overlay(sBuf, 0);

    assertNotNull(signer_name, "Name of signer must be non-null in connection configuration");

    if (strcasecmp(signer_name, publicKeySigner_GetName()) == 0) {
        return SignerType_PublicKeySigner;
    }

    if (strcasecmp(signer_name, symmetricKeySigner_GetName()) == 0) {
        return SignerType_SymmetricKeySigner;
    }

    return SignerType_Unknown;
}
