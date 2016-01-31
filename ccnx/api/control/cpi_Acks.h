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
 * @file cpi_Acks.h
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
#ifndef libccnx_cpi_Acks_h
#define libccnx_cpi_Acks_h

#include <stdbool.h>
#include <stdint.h>

#include <parc/algol/parc_JSON.h>

struct control_plane_ack;
typedef struct control_plane_ack CPIAck;

#define cpiAck      "CPI_ACK"
#define cpiSeqnum   "SEQUENCE"

/**
 * Create a CPIAck instance from a PARCJSON instance.
 *
 * @param [in] json A pointer to a valid PARCJSON instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a valid CPIAck instance.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
CPIAck *cpiAcks_ParseJSON(const PARCJSON *json);

/**
 * Create a CPIAck instance.
 *
 * @param [in] sequenceNumber The sequence number for the ACK.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a valid CPIAck instance.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 */
CPIAck *cpiAcks_Create(uint64_t sequenceNumber);

/**
 * Create a CPIAck instance, from a template of the original request.
 *
 * @param [in] originalRequest A pointer to a valid PARCJSON instance containint he original request.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a valid PARCJSON instance.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 */
PARCJSON *cpiAcks_CreateAck(const PARCJSON *originalRequest);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 */
PARCJSON *cpiAcks_CreateNack(const PARCJSON *request);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool cpiAcks_IsAck(const PARCJSON *json);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 */
uint64_t cpiAcks_GetAckOriginalSequenceNumber(const PARCJSON *json);
#endif // libccnx_cpi_Acks_h
