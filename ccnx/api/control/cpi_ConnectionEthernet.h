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
 * @file cpi_ConnectionEthernet.h
 * @brief Represents an ethernet connection
 *
 * An ethernet connection is a (local interface name, remote mac address, ethertype) tuple.  A unicast
 * connection, for example, could be ("em3", 3c:15:c2:e7:c5:ca, 0x0801).  The broadcast connection would
 * be ("em3", ff:ff:ff:ff:ff:ff, 0x0801).  You could also use group mac addresses.
 *
 * Creating an ethernet connetion in the forwarder sets up an entry in the connection table that
 * you an then attach routes to.  For example, you could add a route to /foo via the connection
 * ("em3", 3c:15:c2:e7:c5:ca, 0x0801), in which case an Interest would be unicast that way.  A route
 * to a broadcast or group address would broadcast the interest.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef CCNx_Control_API_cpi_ConnectionEthernet_h
#define CCNx_Control_API_cpi_ConnectionEthernet_h

struct cpi_connection_ethernet;
typedef struct cpi_connection_ethernet CPIConnectionEthernet;

#include <ccnx/api/control/cpi_Address.h>
#include <ccnx/api/control/cpi_ControlMessage.h>

/**
 * Creates a CPIConnectionEthernet object
 *
 * The symbolic name represents this connection and may be used by other commands.  It must be
 * unique, otherwise the command will fail when sent to the forwarder.
 *
 * @param [in] interfaceName The name of the local interface
 * @param [in] peerLinkAddress The link layer address of the peer (stores a reference to it)
 * @param [in] ethertype The ethertype to use (host byte order)
 * @param [in] symbolic The user-defined symbolic name
 *
 * @return non-null An Allocated object
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnectionEthernet *cpiConnectionEthernet_Create(const char *interfaceName, CPIAddress *peerLinkAddress, uint16_t ethertype, const char *symbolic);

/**
 * Releases a reference count to the object
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in,out] etherConnPtr A pointer to an etherConn object, will be null'd.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void cpiConnectionEthernet_Release(CPIConnectionEthernet **etherConnPtr);

/**
 * Determine if two CPIConnectionEthernet instances are equal.
 *
 * Two CPIConnectionEthernet instances are equal if, and only if,
 * they are either both null or both non-null and compare
 * as equal field-for-field over (interfaceName, peerLinkAddress, ethertype, symbolic).
 *
 * The interface name is case sensitive, so "ETH0" is not the same as "eth0".
 *
 *
 * The following equivalence relations on non-null `CPIConnectionEthernet` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIConnectionEthernet_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `cpiConnectionEthernet_Equals(x, y)` must return true if and only if
 *        `cpiConnectionEthernet_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiConnectionEthernet_Equals(x, y)` returns true and
 *        `cpiConnectionEthernet_Equals(y, z)` returns true,
 *        then  `cpiConnectionEthernet_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiConnectionEthernet_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiConnectionEthernet_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIConnectionEthernet` instance.
 * @param b A pointer to a `CPIConnectionEthernet` instance.
 * @return true if the two `CPIConnectionEthernet` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIConnectionEthernet *a = cpiConnectionEthernet_Create();
 *    CPIConnectionEthernet *b = cpiConnectionEthernet_Create();
 *
 *    if (cpiConnectionEthernet_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */

bool cpiConnectionEthernet_Equals(const CPIConnectionEthernet *a, const CPIConnectionEthernet *b);

/**
 * Creates a control message to add the connection
 *
 * An add message indicates to the forwarder that it should add the corresponding
 * Ethernet connection.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return non-null a CPI control message
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *cpiConnectionEthernet_CreateAddMessage(const CPIConnectionEthernet *etherConn);

/**
 * Creates a control message to remove the connection
 *
 * A remove message indicates to the forwarder that it should remove the corresponding
 * Ethernet connection.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return non-null a CPI control message
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *cpiConnectionEthernet_CreateRemoveMessage(const CPIConnectionEthernet *etherConn);

/**
 * Checks if the control message is an Add command
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return true Message is an Add command for a ConnectionEthernet
 * @return false Message is not an Add command for a ConnectionEthernet
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool cpiConnectionEthernet_IsAddMessage(const CCNxControl *control);

/**
 * Checks if the message is a Remove command
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] control A CCNx Control message
 *
 * @return true Message is an Remove command for a ConnectionEthernet
 * @return false Message is not Remove Add command for a ConnectionEthernet
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool cpiConnectionEthernet_IsRemoveMessage(const CCNxControl *control);

/**
 * Creates an object from the control message
 *
 * The object does not carry any sense of Add or Remove, that is only part of the
 * Control message.
 *
 * @param [in] control A CCNx Control message
 *
 * @return non-null An Allocated object
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnectionEthernet *cpiConnectionEthernet_FromControl(const CCNxControl *control);

/**
 * Returns the interface name
 *
 * The caller should duplicate the string if it will be stored.
 *
 * @param [in] etherConn An allocated CPIConnectionEthernet
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiConnectionEthernet_GetInterfaceName(const CPIConnectionEthernet *etherConn);

/**
 * Returns the symbolic name
 *
 * The caller should duplicate the string if it will be stored.
 *
 * @param [in] etherConn An allocated CPIConnectionEthernet
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiConnectionEthernet_GetSymbolicName(const CPIConnectionEthernet *etherConn);

/**
 * Returns the peer link address
 *
 * Returns the peer's link address (e.g. 48-bit MAC address).  The caller should
 * acquire its own reference if he address will be stored externally to the
 * CPIConnectionEthernet.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return non-null The peer's link address
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIAddress *cpiConnectionEthernet_GetPeerLinkAddress(const CPIConnectionEthernet *etherConn);

/**
 * Returns the ethertype to use
 *
 * The ethertype will be in host byte order.
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
uint16_t cpiConnectionEthernet_GetEthertype(const CPIConnectionEthernet *etherConn);
#endif // CCNx_Control_API_cpi_ConnectionEthernet_h
