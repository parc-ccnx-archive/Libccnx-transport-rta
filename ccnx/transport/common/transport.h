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
 * @file transport.h
 * @brief Defines the Transport API from the App API.
 *
 * Application interfaces use this API to communicate
 * with the transport.
 *
 * An API will call transport_Create(type), to create
 * a transport of the given type.  Only type TRANSPORT_RTA
 * is supported at this time.  Only one transport may exist,
 * so multiple calls to transport_Create() will return
 * a reference counted pointer to the same transport.
 * When an API is done, it should call transport_Destroy().
 *
 * An API opens connections with the forwarder via
 * transport_Open(PARCJSON *).  The JSON dictionary defines
 * the properties of the protocol stack associated with the
 * connection.  When done, the API should call transport_Close()
 * on the connection.  Multiple calls with the same JSON
 * definition will return new connecitons using the same
 * protocol stack.
 *
 * See the documentation in transport_rta/core/rta_Component.h
 * about how to write a new component for use in a protocol stack.
 *
 *
 * transport_Open() requires a JSON configuration string that
 * defines the SYSTEM and USER parameters.  SYSTEM parameters
 * define the ProtocolStack.  USER parameters are variations
 * within a ProtocolStack, such as which signing keys to use.
 *
 * \code{.c}
 * {
 *  "SYSTEM" : {
 *      "COMPONENTS"     : [ array of identifiers ],
 *      <component name> : { component parameters },
 *      <component name> : { component parameters }
 *  }
 *
 *  "USER" : {
 *      <component name> : {component parameters},
 *      <component name> : {component parameters},
 *      <component name> : {component parameters}
 *  }
 * }
 * \endcode
 *
 * The COMPONENTS parameter lists the comonents in the protocol stack.
 * The names should be taken from components.h (e.g. API_CONNECTOR).
 *
 * An example would be:
 * \code{.c}
 * {
 *   "SYSTEM" : {
 *       "COMPONENTS"     : [
 *                              "API_CONNECTOR",
 *                              "FC_VEGAS,
 *                              "VERIFY_ENUMERATED",
 *                              "CODEC_TLV",
 *                              "FWD_FLAN"
 *                          ],
 *       "FWD_FLAN" : { "port" : 1234 },
 *       "FC_VEGAS" : { "max_cwind": 65536 }
 *  }
 *
 *   "USER" : {
 *      "CODEC_TLV" : {
 *          "SET_SIGNING_KEYSTORE" : {
 *              "KEYSTORE_NAME" : "/Users/alice/.ccnxkeystore",
 *              "KEYSTORE_PASSWD": "1234abcd"
 *          }
 *      }
 *
 *      "VERIFY_ENUMERATED" : {
 *          "ADD_TRUSTED_CERTS" : [
 *               <PEM encoded X.509 cert>,
 *               <PEM encoded X.509 cert>,
 *               <PEM encoded X.509 cert> ]
 *           "ADD_TRUSTED_ISSUERS" : [
 *              <PEM encoded X.509 cert> ]
 *           "ADD_TRUSTED_KEYS" : [
 *              <PEM encoded RSA public key>,
 *              <PEM encoded DSA public key>]
 *          }
 *      }
 * }
 * \endcode
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef CCNX_TRANSPORT_H
#define CCNX_TRANSPORT_H

#include <ccnx/transport/common/transport_MetaMessage.h>

#include <ccnx/common/internal/ccnx_TlvDictionary.h>

#include <ccnx/transport/common/transport_Message.h>
#include <ccnx/transport/common/ccnx_TransportConfig.h>

struct transport_context;
typedef struct transport_context TransportContext;

typedef enum {
    TRANSPORT_RTA
} TransportTypes;

typedef enum TransportIOStatus {
    TransportIOStatus_Success = 0,
    TransportIOStatus_Error = 1,
    TransportIOStatus_Timeout = 2
} TransportIOStatus;

typedef uint64_t CCNxStackTimeout;

/**
 * @def CCNxStackTimeout_Never
 * The receive function is a blocking read that never times out.
 */
#define CCNxStackTimeout_Never NULL

/*
 * @def CCNxStackTimeout_Immediate
 * The receive function is a non-blocking read that immediately either returns a message or nothing.
 * Equivalent to StackTimeout_MicroSeconds(0)
 */
#define CCNxStackTimeout_Immediate (&(uint64_t) { 0 })

/*
 * @def CCNxStackTimeout_MicroSeconds
 * The receive function is a blocking read that either waits no longer than the specified number of microseconds or a message,
 * whichever comes first.
 */
#define CCNxStackTimeout_MicroSeconds(_usec_) (&(uint64_t) { _usec_ })


/**
 * Initialize transport.  Creates a thread of execution,
 * you only need one of these.
 *
 * You can only have one of these.  Multiple calls return a
 * reference to the existing one (if same type) or an error.
 *
 * NULL means error.
 */
TransportContext *Transport_Create(TransportTypes type);

/**
 * Open a descriptor.  You may use a select(2) or poll(2) on it, but
 * you must only use Transport_{Send, Recv, Close} to modify it.
 *
 * All transport operations are non-blocking.
 *
 * Transport will take ownership of the transportConfig and destroy it and
 * everyting contained in it.
 *
 * Generate the configuration based on your stacks configuration
 * methods.  For RTA, they are in transport_rta/config/.
 *
 * @param [in] transportConfig the transport configuration object
 *
 * @return the newly opened descriptor
 *
 * @see Transport_Close
 */
int Transport_Open(CCNxTransportConfig *transportConfig);

/**
 * Send a `CCNxMetaMessage` to the transport. The CCNxMetaMessage instance is acquired by
 * the stack and can be released by the caller immediately after sending if desired.
 *
 * The CCNxMetaMessage may be a PARCJSON object to modify USER stack parameters.
 *
 * @param [in] desc the file descriptor (e.g. one end of a socket) in to which to write he CCNxMetaMessage.
 * @param [in] msg_in A CCNxMetaMessage instance to send.
 *
 * @return 0 if the message was succesfully sent.
 * @return -1 and sets errno, otherwise. errno will be set to EWOULDBLOCK if it would block.
 *
 * Example:
 * @code
 * {
 *     CCNxMetaMessage *msg = ccnxMetaMessage_CreateFromContentObject(contentObject);
 *
 *     int status = Transport_Send(desc, msg);
 *
 *     ccnxMetaMessage_Release(&msg);
 * }
 * @endcode
 *
 * @see ccnxMetaMessage_Release
 */
int Transport_Send(int desc, CCNxMetaMessage *msg_in);

/**
 * Receive a `CCNxMetaMessage` from the transport.  The caller is responsible
 * for calling {@link ccnxMetaMessage_Release} on the message, if successful.
 *
 * @param [in] desc the file descriptor (e.g. one end of a socket) from which to read the CCNxMetaMessage.
 * @param [in] msg_in A CCNxMetaMessage instance to send.
 *
 * @return 0 if the message was succesfully sent.
 * @return -1 and sets errno, otherwise. errno will be set to EWOULDBLOCK if the call would block
 *         or if SO_RCVTIMEO is exceeded on the underlying socket.
 *
 * Example:
 * @code
 * {
 *     CCNxMetaMessage *msg;
 *     int status = Transport_Recv(desc, &msg);
 *
 *     if (status == 0) {
 *         // do things
 *
 *         ccnxMetaMessage_Release(&msg);
 *     }
 * }
 * @endcode
 *
 * @see ccnxMetaMessage_Release
 */
TransportIOStatus Transport_Recv(int desc, CCNxMetaMessage **msg_out);

/**
 * Closes a descriptor.  Close is immediate, any pending data is lost.
 *
 * @param [in] desc the descriptor to close
 *
 * @return 0 on success (the descriptor exists and was open)
 *
 * @see Transport_Open
 */
int Transport_Close(int desc);

/**
 * Pass a transport-specific command to the underlying framework.
 * It must be in a "TransportTypes" format that your chosen
 * transport understands.
 */
int Transport_PassCommand(void *stackCommand);

/**
 * Destroy a TransportContext instance.  Shuts done all descriptors and any pending data is lost.
 *
 * @param [in] ctxP A pointer to a pointer to the TransportContext instance to release.
 */
void Transport_Destroy(TransportContext **ctxP);
#endif // CCNX_TRANSPORT_H
