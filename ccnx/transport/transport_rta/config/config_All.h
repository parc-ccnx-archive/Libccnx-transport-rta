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
//  config_all.h
//  Libccnx
//
//  Created by Mosko, Marc <Marc.Mosko@parc.com> on 10/24/13.

/**
 * This is a convenience header for an API that wishes to import all the standard
 * component configuration headers.
 */

#ifndef Libccnx_config_all_h
#define Libccnx_config_all_h

#include <ccnx/transport/common/ccnx_TransportConfig.h>

#include <ccnx/transport/transport_rta/config/config_ApiConnector.h>

#include <ccnx/transport/transport_rta/config/config_Codec_Tlv.h>
#include <ccnx/transport/transport_rta/config/config_CryptoCache.h>

#include <ccnx/transport/transport_rta/config/config_FlowControl_Vegas.h>
#include <ccnx/transport/transport_rta/config/config_Forwarder_Local.h>
#include <ccnx/transport/transport_rta/config/config_Forwarder_Metis.h>

#include <ccnx/transport/transport_rta/config/config_InMemoryVerifier.h>

#include <ccnx/transport/transport_rta/config/config_ProtocolStack.h>
#include <ccnx/transport/transport_rta/config/config_PublicKeySignerPkcs12Store.h>

#include <ccnx/transport/transport_rta/config/config_Signer.h>
#include <ccnx/transport/transport_rta/config/config_SymmetricKeySignerFileStore.h>

#include <ccnx/transport/transport_rta/config/config_TestingComponent.h>
#endif
