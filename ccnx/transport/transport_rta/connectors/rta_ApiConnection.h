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
 * @file rta_ApiConnection.h
 * @brief Implementation of the API connection
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef TransportRTA_rta_ApiConnection_h
#define TransportRTA_rta_ApiConnection_h

struct rta_api_connection;
typedef struct rta_api_connection RtaApiConnection;

#include <ccnx/transport/transport_rta/core/rta_Connection.h>

RtaApiConnection *rtaApiConnection_Create(RtaConnection *connection);
void rtaApiConnection_Destroy(RtaApiConnection **rtaApiConnectionPtr);

/**
 * Sends a TransportMessage up to the API
 *
 * Decapsulates the ccnx message and sends it up to the API.  It will destroy the TransportMessage wrapper.
 *
 * @param [in] apiConnection The API connection to write to
 * @param [in] tm The transport message to send
 * @param [in] stats The statistics counter to increment on success
 *
 * @return true Transport message written
 * @return false Transport message not written (but still destroyed)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool rtaApiConnection_SendToApi(RtaApiConnection *apiConnection, TransportMessage *tm, RtaComponentStats *stats);

/**
 * Block data flow in the DOWN direction
 *
 * To block in the DOWN direction, we disable READ events on the API's buffer
 *
 * @param [in] apiConnection The API Connector's connection state
 * @param [in] conn The RTA Connection
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaApiConnection_BlockDown(RtaApiConnection *apiConnection);

/**
 * Unblock data flow in the DOWN direction
 *
 * To unblock in the DOWN direction, we enable READ events on the API's buffer
 *
 * @param [in] apiConnection The API Connector's connection state
 * @param [in] conn The RTA Connection
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaApiConnection_UnblockDown(RtaApiConnection *apiConnection);
#endif
