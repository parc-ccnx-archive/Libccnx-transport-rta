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
 * @file cpi_ControlFacade.h
 * @ingroup Utility
 * @brief <#Brief Description#>
 *
 * A ControlFacade has several flavors.  A Notification is a spontaneous message
 * sent as an indication of some state or condition.  A CPI (ControlPlaneInterface)
 * message is a Request/Reponse protocol used to manage the Transport.
 *
 * The ccnxControlFacade takes ownership of the JSON object
 * and will destroy it when it's Destroy is called.
 *
 * If put inside a CCNxMetaMessage and sent to the Transport, the Transport
 * takes ownership of the CCNxMetaMessage and will thus be responsible for
 * destroying the object.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_ccnx_ControlFacade_h
#define libccnx_ccnx_ControlFacade_h

#include <ccnx/common/ccnx_Name.h>
#include <ccnx/api/control/cpi_ControlMessage.h>

#include <ccnx/common/internal/ccnx_TlvDictionary.h>

typedef enum {
    CCNxControlMessage_Unknown = 0,
    CCNxControlMessage_CPI = 1,
    CCNxControlMessage_Notify = 2
} CCNxControlFacadeType;

// =====================

/**
 * Creates a Nofification control message from the supplied JSON object.
 *
 * The newly created instance must eventually be released by calling
 * {@link ccnxControl_Release}.
 *
 * @param ccnxJson the JSON object to include in the message.
 * @return A `CCNxControl` message.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *ccnxControlFacade_CreateNotification(PARCJSON *ccnxJson);

/**
 * Creates a CPI message from the supplied JSON object.
 *
 * The newly created instance must eventually be released by calling
 * {@link ccnxControl_Release}.
 *
 * @param ccnxJson the JSON to include with the message.
 * @return A `CCNxControl` message.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *ccnxControlFacade_CreateCPI(PARCJSON *ccnxJson);

// =====================
// Getters

/**
 * Return a pointer to the JSON object contained in the control message.
 *
 *   <#Discussion#>
 *
 * @param controlDictionary the control message to retrieve the JSON from.
 * @return the PARCJSON object from the control message.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *ccnxControlFacade_GetJson(const CCNxTlvDictionary *controlDictionary);

/**
 * Test whether a control message is a Notification.
 *
 *   <#Discussion#>
 *
 * @param controlDictionary the control message to test.
 * @return true if the control message is a Notification.
 * @return false if the control message is not a Notification.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool ccnxControlFacade_IsNotification(const CCNxTlvDictionary *controlDictionary);

/**
 * Test whether a control message is a CPI (Control Plane Interface) message.
 *
 *   <#Discussion#>
 *
 * @param controlDictionary the control message to test.
 * @return true if the control message is a CPI message.
 * @return false if the control message is not a CPI message.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool ccnxControlFacade_IsCPI(const CCNxTlvDictionary *controlDictionary);


// =====================
// Miscellaneous

/**
 * Print a human readable representation of the given `CCNxTlvDictionary` representing
 * a control message.
 *
 * @param [in] name A pointer to the `CCNxTlvDictionary` instance representing a CCNxControl.
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 *
 * Example:
 * @code
 * {
 *     CCNxTlvDictionary *controlMessage = ccnxControlFacade_CreateCPI(...);
 *     ccnxControlFacade_Display(controlMessage);
 *     ...
 * }
 * @endcode
 */
void ccnxControlFacade_Display(const CCNxTlvDictionary *controlDictionary, int indentation);

/**
 * Produce a null-terminated string representation of the specified CCNxTlvDictionary instance
 * representing a control message.
 *
 * The non-null result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] name A pointer to the `CCNxTlvDictionary` instance representing a CCNxControl.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated,
 *         null-terminated C string that must be deallocated via `parcMemory_Deallocate()`.
 *
 * Example:
 * @code
 * {
 *      char *desc = ccnxControlFacade_ToString(controlDictionary);
 *      printf("%s\n", desc);
 *      parcMemory_Deallocate((void **) &desc);
 *
 *      ccnxTlvDictionary_Release(&control);
 * }
 * @endcode
 *
 * @see {@link ccnxControlFacade_Display}
 * @see {@link parcMemory_Deallocate}
 */
char *ccnxControlFacade_ToString(const CCNxTlvDictionary *controlDictionary);

/**
 * Assert that an instance of `CCNxTlvDictionary` is a valid control message.
 *
 * If the instance is not valid, terminate via {@link trapIllegalValue}
 *
 * Valid means the internal state of the type is consistent with its
 * required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] name A pointer to a `CCNxName` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxControl *controlMessage = ccnxControlFacade_CreateCPI(...);
 *     ccnxControlFacade_AssertValid(controlMessage);
 *     ...
 * }
 * @endcode
 */
void ccnxControlFacade_AssertValid(const CCNxTlvDictionary *controlDictionary);
#endif // libccnx_ccnx_ControlFacade_h
