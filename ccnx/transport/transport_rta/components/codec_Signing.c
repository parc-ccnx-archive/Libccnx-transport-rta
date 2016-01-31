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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_PublicKeySignerPkcs12Store.h>
#include <parc/security/parc_SymmetricSignerFileStore.h>
#include <parc/security/parc_CryptoHashType.h>

#include <ccnx/transport/transport_rta/config/config_Signer.h>
#include "codec_Signing.h"

PARCSigner *
component_Codec_GetSigner(RtaConnection *conn)
{
    PARCSigner *signer = NULL;

    SignerType signertype = signer_GetImplementationType(rtaConnection_GetParameters(conn));

    switch (signertype) {
        case SIGNER_SymmetricKeySignerFileStore: {
            struct symmetrickeysigner_params params;
            bool success = symmetricKeySignerFileStore_GetConnectionParams(rtaConnection_GetParameters(conn), &params);
            assertTrue(success, "Could not retrieve publicKeySignerPkcs12Store_GetConnectionParams");

            signer = parcSigner_Create(parcSymmetricSignerFileStore_OpenFile(params.filename, params.password, PARC_HASH_SHA256));
            assertNotNull(signer, "got null opening FileKeystore '%s'\n", params.filename);
            break;
        }

        case SIGNER_PublicKeySignerPkcs12Store: {
            struct publickeysigner_params params;
            bool success = publicKeySignerPkcs12Store_GetConnectionParams(rtaConnection_GetParameters(conn), &params);
            assertTrue(success, "Could not retrieve publicKeySignerPkcs12Store_GetConnectionParams");

            signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(params.filename, params.password, PARC_HASH_SHA256));
            assertNotNull(signer, "got null opening FileKeystore '%s'\n", params.filename);
            break;
        }

        default:
            assertTrue(0, "Unsupported signer type %d", signertype);
    }

    assertNotNull(signer, "Did not match a known signer");
    return signer;
}
