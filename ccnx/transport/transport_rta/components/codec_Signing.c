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

#include <parc/security/parc_PublicKeySigner.h>
#include <parc/security/parc_Pkcs12KeyStore.h>
#include <parc/security/parc_SymmetricKeySigner.h>
#include <parc/security/parc_SymmetricKeyStore.h>
#include <parc/security/parc_KeyStore.h>
#include <parc/security/parc_Signer.h>
#include <parc/security/parc_CryptoHashType.h>

#include <ccnx/transport/transport_rta/config/config_Signer.h>
#include "codec_Signing.h"

PARCSigner *
component_Codec_GetSigner(RtaConnection *conn)
{
    PARCSigner *signer = NULL;

    SignerType signertype = signer_GetImplementationType(rtaConnection_GetParameters(conn));

    switch (signertype) {
        case SignerType_SymmetricKeySigner: {
            struct symmetrickeysigner_params params;
            bool success = symmetricKeySigner_GetConnectionParams(rtaConnection_GetParameters(conn), &params);
            assertTrue(success, "Could not retrieve symmetricKeySigner_GetConnectionParams");

            PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_OpenFile(params.filename, params.password, PARC_HASH_SHA256);
            PARCSymmetricKeySigner *symmetricKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARC_HASH_SHA256);
            parcSymmetricKeyStore_Release(&symmetricKeyStore);

            signer = parcSigner_Create(symmetricKeySigner, PARCSymmetricKeySignerAsSigner);
            parcSymmetricKeySigner_Release(&symmetricKeySigner);
            assertNotNull(signer, "got null opening FileKeystore '%s'\n", params.filename);
            break;
        }

        case SignerType_PublicKeySigner: {
            struct publickeysigner_params params;
            bool success = publicKeySigner_GetConnectionParams(rtaConnection_GetParameters(conn), &params);
            assertTrue(success, "Could not retrieve publicKeySigner_GetConnectionParams");

            PARCPkcs12KeyStore *pkcs12KeyStore = parcPkcs12KeyStore_Open(params.filename, params.password, PARC_HASH_SHA256);
            PARCKeyStore *keyStore = parcKeyStore_Create(pkcs12KeyStore, PARCPkcs12KeyStoreAsKeyStore);
            parcPkcs12KeyStore_Release(&pkcs12KeyStore);
            PARCPublicKeySigner *publicKeySigner = parcPublicKeySigner_Create(keyStore, PARCSigningAlgorithm_RSA, PARC_HASH_SHA256);
            parcKeyStore_Release(&keyStore);

            signer = parcSigner_Create(publicKeySigner, PARCPublicKeySignerAsSigner);
            parcPublicKeySigner_Release(&publicKeySigner);
            assertNotNull(signer, "got null opening FileKeystore '%s'\n", params.filename);
            break;
        }

        default:
            assertTrue(0, "Unsupported signer type %d", signertype);
    }

    assertNotNull(signer, "Did not match a known signer");
    return signer;
}
