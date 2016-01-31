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
 * @file cpi_CancelFlow.h
 * @brief Cancel a "flow"
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_CancelFlow_h
#define libccnx_cpi_CancelFlow_h

#include <ccnx/api/control/cpi_ControlMessage.h>

#include <ccnx/common/ccnx_Name.h>

/**
 * Creates a CPI reqeust to cancel a flow
 *
 * Will return an asynchronous ACK or NACK.
 *
 * @param name The CCNxName of the flow to cancel.
 * @return A pointer to a valid CPIControlMessage
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *cpiCancelFlow_CreateRequest(const CCNxName *name);

/**
 * Creates a CPI reqeust to cancel a flow
 *
 * @param [in] name The CCNxName of the flow to cancel.
 *
 * @return NULL An error occurred
 * @return non-NULL A pointer to a valid PARCJSON instance.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCJSON *cpiCancelFlow_Create(const CCNxName *name);

/**
 * Return the CCNxName associated with the given control message
 *
 * @param controlMessage A pointer to a control message.
 * @return A pointer to a valid CCNxName instance.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxName *cpiCancelFlow_GetFlowName(const PARCJSON *controlMessage);

/**
 * Return the name associated with the message
 *
 * @param controlMessage A pointer to a control message.
 * @return A pointer to a valid CCNxName instance.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxName *cpiCancelFlow_NameFromControlMessage(CCNxControl *controlMessage);

/**
 * Given a CPI response (ACK or NACK) return the success state
 *
 * @param controlMessage A pointer to a control message.
 * @return true if the control message signals success.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool cpiCancelFlow_SuccessFromResponse(CCNxControl *controlMessage);

/**
 * The CPI tag used for cancel flow
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiCancelFlow_CancelFlowJsonTag(void);
#endif // libccnx_cpi_CancelFlow_h
