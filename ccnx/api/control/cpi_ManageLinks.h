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
 * @file cpi_ManageLinks.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_ManageLinks_h
#define libccnx_cpi_ManageLinks_h

#include <ccnx/api/control/cpi_ConnectionList.h>
#include <ccnx/api/control/cpi_ControlMessage.h>
#include <ccnx/api/control/cpi_InterfaceType.h>
#include <ccnx/api/control/cpi_InterfaceIPTunnel.h>
#include <ccnx/api/control/cpi_InterfaceIPTunnelList.h>
#include <ccnx/api/control/cpi_InterfaceSet.h>

/**
 * Create a control message that asks the forwarder to return a list of connections
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
PARCJSON *cpiLinks_CreateConnectionListRequest();

/**
 * Returns a native object from a control message of connections
 *
 *   The decoder of the response to <code>cpiLinks_CreateConnectionListRequest()</code>
 *
 * @param <#param1#>
 * @return An allocated object, you must destroy it.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIConnectionList *cpiLinks_ConnectionListFromControlMessage(CCNxControl *response);

/**
 * Generate a request for a list of all interfaces
 *
 *   The transport should resond with a CPI Response message.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *cpiLinks_CreateInterfaceListRequest(void);

/**
 * Parse a control message into a list of interfaces
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
CPIInterfaceSet *cpiLinks_InterfacesFromControlMessage(CCNxControl *response);

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
PARCJSON *cpiLinks_CreateIPTunnel(const CPIInterfaceIPTunnel *iptun);

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
CPIInterfaceIPTunnel *cpiLinks_CreateIPTunnelFromControlMessage(CCNxControl *response);

/**
 * Set an interface to UP or DOWN
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
CCNxControl *cpiLinks_SetInterfaceState(unsigned ifidx, CPIInterfaceStateType state);

/**
 * Removes an interface
 *
 *   If it is a virtual interface created through the ControlPlaneInterface, it
 *   is complete removed.
 *
 *   Trying to remove a physical interface will result in it going down, but it
 *   might not be removed from the system.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CCNxControl *cpiLinks_RemoveInterface(unsigned ifidx);

/**
 * The key name for an InterfaceList branch
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Do not free it.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiLinks_InterfaceListJsonTag();

/**
 * The string tag used in JSON for a Create Tunnel request
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Do not free it
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiLinks_CreateTunnelJsonTag();

/**
 * The string tag used in JSON for a Remove Tunnel request
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Do not free it
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiLinks_RemoveTunnelJsonTag();

/**
 * The string tag used in JSON for a Connection List request
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Do not free it
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiLinks_ConnectionListJsonTag();

/**
 * The string tag used in JSON to add an Ethernet connection
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Do not free it
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiLinks_AddEtherConnectionJasonTag();
#endif // libccnx_cpi_ManageLinks_h

