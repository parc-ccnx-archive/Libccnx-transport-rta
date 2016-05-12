/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 * 
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @file notify_Status.h
 *
 * @brief
 * An API to handle notifications from the Transport.
 *
 * These notifications are specific to the RTA Transport, in that they use the Component  model and Component names.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef Libccnx_notifyStatus_h
#define Libccnx_notifyStatus_h

#include <parc/algol/parc_JSON.h>
#include <ccnx/common/ccnx_Name.h>

struct notify_status;
/**
 * @typedef NotifyStatus
 * @brief Notifications from Transport
 */
typedef struct notify_status NotifyStatus;

// This needs to be replaced with a more sophisticated encoding scheme that individual stack components can use.  Case 1035
/**
 * @typedef NotifyStatusCode
 * @brief Codes for Notify Status
 */
typedef enum {
    // TRANSPORT_READY                       = 1,  // returned when Transport_Create finished
    // TRANSPORT_DESTROYED                   = 2,  // when Transport_Destroy is done
    notifyStatusCode_OPEN_ERROR              = 3,  // error when opening a connection stack
    notifyStatusCode_CONNECTION_OPEN         = 4,  // returned when a connection is opened
    notifyStatusCode_CONNECTION_CLOSED       = 5,  // returned when close is finished
    notifyStatusCode_FORWARDER_NOT_AVAILABLE = 6,  // connection problem with forwarder
    notifyStatusCode_FLOW_CONTROL_STARTED    = 7,  // when flow control starts on a name
    notifyStatusCode_FLOW_CONTROL_FINISHED   = 8,  // after final block is passed up
    notifyStatusCode_FLOW_CONTROL_ERROR      = 9,  // some hard error on the name
    notifyStatusCode_ENCODING_ERROR          = 10, // something bad in the codec
    notifyStatusCode_SIGNING_ERROR           = 11, // error signing
    notifyStatusCode_SEND_ERROR              = 12, // some other "down" stack error
} NotifyStatusCode;

/**
 * @typedef NotifyStatusDirection
 * @brief The direction of the NotifyStatus
 */
typedef enum {
    notifyStatusDirection_UPSTACK,
    notifyStatusDirection_DOWNSTACK
} NotifyStatusDirection;

/**
 * Create an instance of `NotifyStatus`.
 *
 * @param [in] apiFd The corresponding api file descriptor.
 * @param [in] code The NotifyStatusCode for this status.
 * @param [in] name An associated CCNxName
 * @param [in] message An (optional) string message
 *
 * @return NULL An error occurred
 * @return non-NULL A pointer to a valid NotifyStatus instance that must be released via notifyStatus_Release().
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *expected = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 * }
 * @endcode
 *
 * @see {@link notifyStatus_Release}
 */
NotifyStatus *notifyStatus_Create(int apiFd, NotifyStatusCode code, CCNxName *name, const char *message);

/**
 * Increase the number of references to a `NotifyStatus`.
 *
 * Note that new `NotifyStatus` is not created,
 * only that the given `NotifyStatus` reference count is incremented.
 * Discard the reference by invoking {@link notifyStatus_Release}.
 *
 * @param [in] status A pointer to a `NotifyStatus` instance.
 * @return The input `NotifyStatus` pointer.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     NotifyStatus *x_2 = notifyStatus_Acquire(status);
 *
 *     notifyStatus_Release(&status);
 *     notifyStatus_Release(&x_2);
 * }
 * @endcode
 */
NotifyStatus *notifyStatus_Acquire(const NotifyStatus *status);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] statusPtr A pointer to a pointer to the instance to release.
 *
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     notifyStatus_Release(&status);
 * }
 * @endcode
 */
void notifyStatus_Release(NotifyStatus **statusPtr);

/**
 * Returns true if contents of two NotifyStatus objects are the same.
 *
 * @param [in] x object 1
 * @param [in] y object 2
 *
 * @return true X & Y are equal
 * @return false X & Y are not equal
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status1 = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *     NotifyStatus *status2 = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     if (notifyStatus_Equals(status1, status2)) {
 *          ...
 *     }
 * }
 * @endcode
 */
bool notifyStatus_Equals(const NotifyStatus *x, const NotifyStatus *y);

/**
 * Print a human readable representation of the given `NotifyStatus`.
 *
 * @param [in] status A pointer to the instance to display.
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     notifyStatus_Display(status, 0);
 *
 *     notifyStatus_Release(&status);
 * }
 * @endcode
 */
void notifyStatus_Display(const NotifyStatus *status, int indentation);

/**
 * Get the associated file descriptor of the given `NotifyStatus` instance.
 *
 * @param [in] status A pointer to a valid instance of `NotifyStatus`.
 *
 * @return The associated file descriptor of this NotifyStatus instance.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     int fd = notifyStatus_GetFiledes(status);
 * }
 * @endcode
 */
int notifyStatus_GetFiledes(const NotifyStatus *status);

/**
 * Get the associated {@link NotifyStatusCode} of the given `NotifyStatus` instance.
 *
 * @param [in] status A pointer to a valid instance of `NotifyStatus`.
 *
 * @return The associated NotifyStatusCode of the given `NotifyStatus` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     NotifyStatusCode code = notifyStatus_GetStatusCode(status);
 * }
 * @endcode
 */
NotifyStatusCode notifyStatus_GetStatusCode(const NotifyStatus *status);

/**
 * Get the associated {@link CCNxName} of the given `NotifyStatus` instance.
 *
 * @param [in] status A pointer to a valid instance of `NotifyStatus`.
 *
 * @return The `CCNxName` of this `NotifyStatus` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     NotifyStatusCode code = notifyStatus_GetStatusCode(status);
 * }
 * @endcode
 */
CCNxName *notifyStatus_GetName(const NotifyStatus *status);

/**
 * Get the associated {@link CCNxName} of the given `NotifyStatus` instance.
 *
 * @param [in] status A pointer to a valid instance of `NotifyStatus`.
 *
 * @return The message of this NotifyStatus instance.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     NotifyStatusCode code = notifyStatus_GetStatusCode(status);
 * }
 * @endcode
 */
char *notifyStatus_GetMessage(const NotifyStatus *status);

/**
 * Return a {@link PARCJSON} representation of the given `NotifyStatus` instance.
 *
 * @param [in] status A pointer to a valid instance of `NotifyStatus`.
 *
 * @return NULL An error occurred
 * @return non-NULL A pointer to a valid `PARCJSON` instance
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     PARCJSON *json = notifyStatus_ToJSON(status);
 * }
 * @endcode
 */
PARCJSON *notifyStatus_ToJSON(const NotifyStatus *status);

/**
 * Create a new `NotifyStatus` instance from a {@link PARCJSON} instance.
 *
 * @param [in] json A pointer to a `PARCJSON` instance.
 *
 * @return NULL An error occurred
 * @return non-NULL A pointer to a valid `NotifyStatus` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     PARCJSON *json = notifyStatus_ToJSON(status);
 *
 *     NotifyStatus *status2 = notifyStatus_ParseJSON(json)
 * }
 * @endcode
 */
NotifyStatus *notifyStatus_ParseJSON(const PARCJSON *json);

/**
 * Evaluate to `true` if the given `NotifyStatus` indicates a Connection Open.
 *
 * @param [in] status  A pointer to a `NotifyStatus` instance.
 *
 * @return `true` The given `NotifyStatus` indicates a Connection open.
 * @return `false` The given ``NotifyStatus` indicates that the Connection is not open.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     if (notifyStatus_IsConnectionOpen(status)) {
 *         printf("Connection is open\n");
 *     }
 * }
 * @endcode
 */
bool notifyStatus_IsConnectionOpen(const NotifyStatus *status);

/**
 * Return `true` if the given status indicates that the flow control has started.
 *
 * @param [in] status  A pointer to a NotifyStatus instance.
 *
 * @return `true` The given `NotifyStatu`s indicates that flow controller has started
 * @return `false` The given `NotifyStatus` indicates that the flow controller has not started.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromCString("lci:/a/b/c");
 *     NotifyStatus *status = notifyStatus_Create(1, notifyStatus_CONNECTION_OPEN, name, "Good to go");
 *
 *     if (notifyStatus_IsFlowControlStarted(status)) {
 *         printf("Flow controller has started\n");
 *     }
 * }
 * @endcode
 */
bool notifyStatus_IsFlowControlStarted(const NotifyStatus *status);
#endif // Libccnx_notifyStatus_h
