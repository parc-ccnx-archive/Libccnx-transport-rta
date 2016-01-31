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
 * @file controlPlaneInterface.h
 *
 * Based loosely on Netlink (RFC 3549)
 *
 * Constructs `CCNxControl` for common control plane operations.
 * These methods do not actually communicate with the transport.
 * The user must send the message down their protocol stack and await a response.
 *
 * CPI messages have a Type: Request, Response, or ACK.
 * A request may ask for an ACK if successful for commands that would otherwise not
 * produce a response.
 * Request that fail always generate a NACK.
 *
 * All messages must carry a "sequence number" which must be unique within
 * the Transport.
 * Although sequence numbers imply an ordering, they do not imply
 * causality or precedence.
 * They only imply a duplicate.
 *
 * An ACK is a reponse that carries no data, it just ACKs (or NACKs) a sequence number.
 * A field in the ACK indicates it is an error (NACK).  An ACK carries the original
 * request and an optional message.
 *
 * All messages carry a mandatory sequence number,
 * which is unique in all messages.
 * An ACK (or NACK) contains the original message that generated the ACK,
 * including its sequence number.
 * These conventions allow one to implement a reliable CPI messaging system, if desired.
 *
 * The Control Plane operations are:
 *
 * * CPI_INTERFACE_LIST
 *    Return: A response with an array of network interfaces ("interfaceIndex", type, and flags), or a NACK.
 *
 * * CPI_REGISTER
 *    Add a FIB entry with the given CCNxName prefix to the specified interfaceIndex.
 *    The value of "-1" means the current interface.
 *    Return: an ACK (or NACK)
 *
 * * CPI_UNREGISTER
 *    Remove a FIB entry with the given CCNxName prefix from the specified interfaceIndex.
 *    The value of "-1" means the current interface.
 *    Return: an ACK (or NACK)
 *
 * * CPI_FORWARDER_VERSION:
 *    Return: Response (a string) or a NACK.
 *
 * * CPI_ADDRESS
 *    Request: by interfaceIndex
 *    Response: the sockaddr_storage list for the interface, or a NACK
 *
 * * CPI_PREFIX_REGISTRATION_LIST
 *    Request: by interfaceIndex, value "-1" means the current interface
 *    Response: the list of CCNxNames (with their flags) registered on the interface or a NACK
 *
 * * CPI_PAUSE_INPUT
 *    Request: by current connection, causes stack to pause the input (top and bottom)
 *    Response: Forwarder sends ACK up the stack.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
/*
 * This description needs to be revised based on the recent refactoring.
 *
 * The user will be interested in cpi_Forwarding.h and cpi_ManageLinks.h.
 *
 * Case 1027: need to specify the memory management, especially for variable sized messages.
 */

#ifndef libccnx_controlPlaneInterface_h
#define libccnx_controlPlaneInterface_h

#include <ccnx/transport/common/transport_MetaMessage.h>
#include <sys/socket.h>

#include <ccnx/api/control/cpi_ControlMessage.h>
#include <ccnx/api/control/cpi_Forwarding.h>
#include <ccnx/api/control/cpi_ManageLinks.h>
#include <ccnx/api/control/cpi_CancelFlow.h>

typedef enum {
    CPI_REQUEST,
    CPI_RESPONSE,       // a resonse with contents
    CPI_ACK             // a response without contents
} CpiMessageType;

typedef enum {
    CPI_ERROR,                      // a NACK response, carries original message
    CPI_REGISTER_PREFIX,
    CPI_UNREGISTER_PREFIX,
    CPI_FORWARDER_VERSION,
    CPI_INTERFACE_LIST,
    CPI_ADDRESS,
    CPI_PREFIX_REGISTRATION_LIST,
    CPI_PAUSE,
    CPI_FLUSH,
    CPI_CANCEL_FLOW,
    CPI_CREATE_TUNNEL,
    CPI_REMOVE_TUNNEL,
    CPI_CONNECTION_LIST,
    CPI_ADD_ETHER_CONNECTION,
    CPI_ADD_CONNECTION_ETHERNET,
    CPI_REMOVE_CONNECTION_ETHERNET,
    CPI_ADD_LISTENER,
    CPI_REMOVE_LISTENER
} CpiOperation;

typedef enum {
    ACK_ACK,
    ACK_NACK
} CpiAckType;

typedef struct control_plane_information {
    CpiMessageType messageType;
    CpiOperation operation;
    uint64_t serialNumber;
} ControlPlaneInformation;


const char *cpiRequest_GetJsonTag();
const char *cpiResponse_GetJsonTag();

/**
 * Return the name used in the JSON representation for a control message sequence number.
 *
 * @return The name used in the JSON representation for a control message sequence number.
 *
 * Example:
 * @code
 * {
 *cpiSequence_GetJSONTag
 * }
 * @endcode
 */
const char *cpiSequence_GetJSONTag(void);

/**
 * Get the CpiOperation from the given JSON representation of the CPI command.
 *
 * @param [in] json A pointer to a valid PARCJSON instance.
 *
 * @return The CpiOperation specified in the JSON.
 *
 * Example:
 * @code
 * {
 *     const char *sequenceNumberJSONName = cpiSequence_GetJSONTag();
 * }
 * @endcode
 *
 * @see <#references#>
 */
CpiOperation cpi_getCPIOperation2(const PARCJSON *json);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] control A pointer to a valid CCNxControl instance.
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
 * @see <#references#>
 */
CpiOperation cpi_GetMessageOperation(CCNxControl *control);

/**
 * Get the CpiMessageType from the given JSON representation of the CPIMessage
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
 * @see <#references#>
 */
CpiMessageType controlPlaneInterface_GetCPIMessageType(PARCJSON *json);

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
 * @see <#references#>
 */
CpiMessageType cpi_GetMessageType(const CCNxControl *control);

/**
 * Get the sequence number of the given Control Plane Message.
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
 * @see <#references#>
 */
uint64_t controlPlaneInterface_GetSequenceNumber(const PARCJSON *controlPlaneMessage);

/**
 * All CPI messages carry a sequence number.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
uint64_t cpi_GetSequenceNumber(CCNxControl *control);

/**
 * Gererate a CPI request
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
#define CPI_CURRENT_INTERFACE 0x7FFFFFFF

/**
 * Generate a control object to request the forewarder version
 *
 *   Will cause a CPI Response to be sent back, if the forwarder supports the command.
 *   Otherwise, the ForwarderConnector should send a NACK back.
 *
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *cpi_ForwarderVersion(void);

/**
 * Cause the connection to pause input (from the top and bottom).
 * When the ACk arrives back to the top, caller know there are no
 * more data messages in the stack.
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
 * @see <#references#>
 */
PARCJSON *cpi_CreatePauseInputRequest(void);

/**
 * Creates a message that the forwarder connector will ACK.  Once the
 * ACK with the corresponding sequence number is received, the sender knows
 * that all prior messages had been handled by the forwarder connector.
 *
 * @return non-null An allocted JSON object
 * @return null An error
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 * @see <#references#>
 */
PARCJSON *cpi_CreateFlushRequest(void);

/**
 * Given the inner operation member, wrap it in a Request with a sequence number
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
PARCJSON *cpi_CreateRequest(const char *key, PARCJSON *operation);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
CCNxControl *cpi_CreateResponse(CCNxControl *request, PARCJSON *response);

/**
 * Create an acknowledgement to the given request (expressed in JSON).
 *
 * @param [in] request A pointer to a PARCJSON representation of the request to acknowledge.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a PARCJSON instance representing the acknowledgement.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 * @see <#references#>
 */
PARCJSON *cpiAcks_CreateAck(const PARCJSON *request);

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
 * @see <#references#>
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
 *
 * @see <#references#>
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
 * @see <#references#>
 */
uint64_t cpiAcks_GetAckOriginalSequenceNumber(const PARCJSON *json);
#endif // libccnx_controlPlaneInterface_h
