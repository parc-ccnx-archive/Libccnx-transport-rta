/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file cpi_Connection.h
 * @brief Represents a point-to-point tunnel over IP.
 *
 * The carries can be UDP, TCP, or GRE
 *
 * We use InterfaceGeneric to back this type.  We always use 2 addresses in the address list.
 * Address 0 is the source and address 1 is the destination.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_Connection_h
#define libccnx_cpi_Connection_h

#include <ccnx/api/control/cpi_InterfaceType.h>
#include <ccnx/api/control/cpi_Address.h>

struct cpi_connection;
typedef struct cpi_connection CPIConnection;

typedef enum {
    cpiConnection_GRE,
    cpiConnection_TCP,
    cpiConnection_UDP,
    cpiConnection_MULTICAST,
    cpiConnection_L2
} CPIConnectionType;

/**
 * Return a static, nul-terminated C string representing the given `CPIConnectionType`
 *
 * @param type A valid CPIConnectionType value.
 * @return A static, nul-terminated C string
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiConnectionType_ToString(CPIConnectionType type);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnectionType cpiConnectionType_FromString(const char *typeAsString);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBufferComposer *cpiConnectionType_BuildString(CPIConnectionType type, PARCBufferComposer *composer);

/**
 * A representation of a Connection, being two addresses and a type
 *
 *   <#Discussion#>
 *
 * @param ifidx The interface index
 * @param source is the local address
 * @param destination is the remote address
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnection *cpiConnection_Create(unsigned ifidx, CPIAddress *source, CPIAddress *destination, CPIConnectionType connType);


CPIConnection *cpiConnection_Acquire(const CPIConnection *conn);

/**
 * Creates a reference counted copy
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return An allocated copy, you must destroy it
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnection *cpiConnection_Copy(const CPIConnection *conn);

/**
 * Reference counted release
 *
 *   Only on the last reference will the call free the contents.
 *
 * @param <#param1#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void cpiConnection_Release(CPIConnection **connPtr);

/**
 * A connection may be up, down, or don't know state.
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void cpiConnection_SetState(CPIConnection *conn, CPIInterfaceStateType state);

/**
 * Returns the interface index
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return The interface index
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
unsigned cpiConnection_GetIndex(const CPIConnection *conn);

/**
 * The source address
 *
 *   This is not a copy, it is the pointer to what is in the object.  If you
 *   want to save it, make a copy.
 *
 * @param <#param1#>
 * @return Do not destroy it.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const CPIAddress *cpiConnection_GetSourceAddress(const CPIConnection *conn);

/**
 * The destination (remote) address
 *
 *   This is not a copy, it is the pointer to what is in the object.  If you
 *   want to save it, make a copy.
 *
 * @param <#param1#>
 * @return Do not destroy it.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const CPIAddress *cpiConnection_GetDestinationAddress(const CPIConnection *conn);

/**
 * The type of connection
 *
 *   A connection may be a TCP tunnel, UDP tunnel, IP multicast overlay,
 *   PF_LOCAL connection, or a layer 2 connection.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnectionType         cpiConnection_GetConnectionType(const CPIConnection *conn);

/**
 * The connection state, Up, Down, or Don't Know
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterfaceStateType  cpiConnection_GetState(const CPIConnection *conn);

/**
 * Determine if two CPIConnection instances are equal.
 *
 * Two CPIConnection instances are equal if, and only if,
 * (a) the interface index is the same, (b) the connection types are the same,
 * (c) the connection state is the same, (d) the source address are the same, and
 * (e) the destination addresses are the same.
 *
 * The following equivalence relations on non-null `CPIConnection` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIConnection_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `cpiConnection_Equals(x, y)` must return true if and only if
 *        `cpiConnection_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiConnection_Equals(x, y)` returns true and
 *        `cpiConnection_Equals(y, z)` returns true,
 *        then  `cpiConnection_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiConnection_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiConnection_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIConnection` instance.
 * @param b A pointer to a `CPIConnection` instance.
 * @return true if the two `CPIConnection` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIConnection *a = cpiConnection_Create();
 *    CPIConnection *b = cpiConnection_Create();
 *
 *    if (cpiConnection_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool cpiConnection_Equals(const CPIConnection *a, const CPIConnection *b);

/**
 * A JSON representation of the connection
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return An allocated object that you must destroy
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *cpiConnection_ToJson(const CPIConnection *conn);

/**
 * Creates a Connection object based on a JSON representation.
 *
 *   Will assert if there's a parsing error
 *
 * @param <#param1#>
 * @return An allocated connection that you must destroy
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnection *cpiConnection_CreateFromJson(PARCJSON *json);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
char *cpiConnection_ToString(const CPIConnection *connection);
#endif // libccnx_cpi_Connection_h
