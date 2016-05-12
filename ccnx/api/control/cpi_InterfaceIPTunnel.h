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
 * @file cpi_InterfaceIPTunnel.h
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
#ifndef libccnx_cpi_InterfaceIPTunnel_h
#define libccnx_cpi_InterfaceIPTunnel_h

#include <ccnx/api/control/cpi_InterfaceType.h>
#include <ccnx/api/control/cpi_Address.h>

#include <parc/algol/parc_JSON.h>

struct cpi_interface_iptun;
/**
 *
 * @see cpiInterfaceIPTunnel_Create
 */
typedef struct cpi_interface_iptun CPIInterfaceIPTunnel;

typedef enum {
    IPTUN_UDP,
    IPTUN_TCP,
    IPTUN_GRE
} CPIInterfaceIPTunnelType;

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] type A CPIInterfaceIPTunnelType value
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
const char *cpiInterfaceIPTunnel_TypeToString(CPIInterfaceIPTunnelType type);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] str A nul-terminated C string.
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
CPIInterfaceIPTunnelType cpiInterfaceIPTunnel_TypeFromString(const char *str);

/**
 * Creates a representation of an IP tunnel
 *
 * The symblic name will be used in the future to refer to this tunnel.  It must be unique or the forwarder will reject the command.
 *
 * @param [in] ifidx The interface index of the tunnel (may be 0 if not known)
 * @param [in] source The local address and optional port
 * @param [in] destination The remote address and port
 * @param [in] tunnelType The encapsulation protocol
 * @param [in] symbolic The symbolic name to refer to this tunnel (e.g. 'tun2')
 *
 * @return non-null An allocated CPIInterfaceIPTunnel
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
CPIInterfaceIPTunnel *cpiInterfaceIPTunnel_Create(unsigned ifidx, CPIAddress *source, CPIAddress *destination, CPIInterfaceIPTunnelType tunnelType, const char *symbolic);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
CPIInterfaceIPTunnel *cpiInterfaceIPTunnel_Copy(const CPIInterfaceIPTunnel *ipTunnel);
CPIInterfaceIPTunnel *cpiInterfaceIPTunnel_Acquire(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
 * @param [in] state A CPIInterfaceStateType value.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
void cpiInterfaceIPTunnel_SetState(CPIInterfaceIPTunnel *ipTunnel, CPIInterfaceStateType state);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnelPtr A pointer to a pointer to a valid CPIInterfaceIPTunnel instance.
 *
 * @see <#references#>
 */
void cpiInterfaceIPTunnel_Release(CPIInterfaceIPTunnel **ipTunnelPtr);

/**
 * Returns the symbolic name of the tunnel
 *
 * The caller should make a copy of the string if it will be stored.
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
 *
 * @return non-null The symbolic name
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
const char *cpiInterfaceIPTunnel_GetSymbolicName(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
unsigned cpiInterfaceIPTunnel_GetIndex(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
const CPIAddress *cpiInterfaceIPTunnel_GetSourceAddress(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
const CPIAddress *cpiInterfaceIPTunnel_GetDestinationAddress(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
CPIInterfaceIPTunnelType cpiInterfaceIPTunnel_GetTunnelType(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
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
CPIInterfaceStateType cpiInterfaceIPTunnel_GetState(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * Determine if two CPIInterfaceIPTunnel instances are equal.
 *
 *
 * The following equivalence relations on non-null `CPIInterfaceIPTunnel` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIInterfaceIPTunnel_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `cpiInterfaceIPTunnel_Equals(x, y)` must return true if and only if
 *        `cpiInterfaceIPTunnel_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiInterfaceIPTunnel_Equals(x, y)` returns true and
 *        `cpiInterfaceIPTunnel_Equals(y, z)` returns true,
 *        then  `cpiInterfaceIPTunnel_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiInterfaceIPTunnel_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiInterfaceIPTunnel_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIInterfaceIPTunnel` instance.
 * @param b A pointer to a `CPIInterfaceIPTunnel` instance.
 * @return true if the two `CPIInterfaceIPTunnel` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIInterfaceIPTunnel *a = cpiInterfaceIPTunnel_Create();
 *    CPIInterfaceIPTunnel *b = cpiInterfaceIPTunnel_Create();
 *
 *    if (cpiInterfaceIPTunnel_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool cpiInterfaceIPTunnel_Equals(const CPIInterfaceIPTunnel *a, const CPIInterfaceIPTunnel *b);

/**
 * JSON representation
 *
 * <code>
 * { "TUNNEL" :
 *   { "IFIDX" : ifidx,
 *    ["STATE" : "UP" | "DOWN", ]
 *    "TYPE": "UDP" | "TCP" | "GRE",
 *    "SRC" : {srcaddr},
 *    "DST" : {dstaddr}
 *   }
 * }
 * </code>
 *
 * @param [in] ipTunnel A pointer to a valid CPIInterfaceIPTunnel
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *cpiInterfaceIPTunnel_ToJson(const CPIInterfaceIPTunnel *ipTunnel);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] json A pointer to a valid PARCJSON instance.
 *
 * @return non-NULL A pointer to a valid CPIInterfaceIPTunnel instance.
 * @return NULL Memory could not be allocated.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
CPIInterfaceIPTunnel *cpiInterfaceIPTunnel_CreateFromJson(PARCJSON *json);
#endif // libccnx_cpi_InterfaceIPTunnel_h
