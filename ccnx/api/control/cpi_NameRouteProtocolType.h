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
 * @file cpi_NameRouteProtocolType.h
 * @brief Specifies the reason for or creator of a route (i.e. the protocol that created the route)
 *
 * A LOCAL route points to an application running on the localhost.
 *
 * A CONNECTED route exists because the described destination is directly connected to the localhost.  For
 * example, a route to a link local network name would be CONNECTED.
 *
 * A STATIC route is administratively created, such as via the "metis_control" program or via the
 * configuration file.
 *
 * An ACORN route is dynamically created by the ACRON routing protocol.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_NameRouteProtocolType_h
#define libccnx_cpi_NameRouteProtocolType_h



/**
 * @typedef CPINameRouteProtocolType
 * @abstract Enumerates the protocol that created a route
 * @constant cpiNameRouteProtocolType_LOCAL An application running on the localhost
 * @constant cpiNameRouteProtocolType_CONNECTED A directly connected destination
 * @constant cpiNameRouteProtocolType_STATIC Administratively created
 * @constant cpiNameRouteProtocolType_ACORN The ACORN routing protocol
 */
typedef enum {
    cpiNameRouteProtocolType_LOCAL = 0,      // local face to app
    cpiNameRouteProtocolType_CONNECTED = 1,  // directly connected network
    cpiNameRouteProtocolType_STATIC = 2,     // administrative static route
    cpiNameRouteProtocolType_ACORN = 20
} CPINameRouteProtocolType;

/**
 * Return the string representation of the specified `CPINameRouteProtocolType`.
 *
 * The returned string does not need to be freed.
 *
 * @param [in] type The type to represent as a string.
 *
 * @return The string representation of the specified 'CPINameRouteProtocolType'.
 *
 * Example:
 * @code
 * {
 *     CPINameRouteProtocolType type = ROUTE_PROTO_CONNECTED;
 *
 *     char *name = cpiNameRouteProtocolType_ToString(type);
 *
 *     printf("NameRouteProtocolType is %s\n", name);
 * }
 * @endcode
 *
 * @see cpiNameRouteProtocolType_FromString
 */
const char *cpiNameRouteProtocolType_ToString(CPINameRouteProtocolType type);

/**
 * Given a string describing a `CPINameRouteProtocolType`, return the matching `CPINameRouteProtocolType`.
 *
 * If an invalid string is specified, the program will terminate with an IllegalValue exception.
 * Possible values are: "LOCAL", "CONNECTED", "STATIC", and "ACORN".
 *
 * @param [in] str A pointer to a string representation of the desired `CPINameRouteProtocolType`.
 *
 * @return The `CPINameRouteProtocolType` matching the specified string.
 *
 * Example:
 * @code
 * {
 *     CPINameRouteProtocolType type = cpiNameRouteProtocolType_FromString("STATIC");
 * }
 * @endcode
 *
 * @see cpiNameRouteProtocolType_ToString
 */
CPINameRouteProtocolType cpiNameRouteProtocolType_FromString(const char *str);
#endif // libccnx_cpi_NameRouteProtocolType_h
