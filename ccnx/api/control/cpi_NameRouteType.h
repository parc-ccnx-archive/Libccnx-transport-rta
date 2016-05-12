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
 * @file cpi_NameRouteType.h
 * @brief Specifies how a route should be matched
 *
 * NOTE: Only LONGEST_MATCH is currently implemented.
 *
 * A LONGEST_MATCH route is a normal CCNx route entry.  It will match any Interest name that is equal to the route prefix
 * or any Interest name that is equal to the router prefix and has additional name components.  Each name component must be
 * exactly equal on a component-by-component basis.
 *
 * An EXACT_MATCH route will not match any longer names.  An Interest name must exactly match the route prefix.
 *
 * A Default route will be used if there are no other matches.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libccnx_cpi_NameRouteType_h
#define libccnx_cpi_NameRouteType_h

/**
 * @typedef CPINameRouteType
 * @abstract Enumerates the types of route entries
 * @constant cpiNameRouteType_EXACT_MATCH Specifies an exact match route
 * @constant cpiNameRouteType_LONGEST_MATCH Specifies a longest matching prefix entry (a normal CCNx route)
 * @constant cpiNameRouteType_DEFAULT Specifies a default route that is used if no other entries match
 */
typedef enum {
    cpiNameRouteType_EXACT_MATCH = 1,
    cpiNameRouteType_LONGEST_MATCH = 2,
    cpiNameRouteType_DEFAULT = 3
} CPINameRouteType;

/**
 * Return the string representation of the specified `CPINameRouteType`.
 *
 * The returned string does not need to be freed.
 *
 * @param [in] type The type to represent as a string.
 *
 * @return The string representation of the specified 'CPINameRouteType'.
 *
 * Example:
 * @code
 * {
 *     CPINameRouteType type = cpiNameRouteType_LONGEST_MATCH;
 *
 *     char *name = cpiNameRouteType_ToString(type);
 *
 *     printf("NameRouteType is %s\n", name);
 * }
 * @endcode
 *
 * @see cpiNameRouteType_FromString
 */
const char *cpiNameRouteType_ToString(CPINameRouteType type);

/**
 * Given a string describing a `CPINameRouteType`, return the matching `CPINameRouteType`.
 *
 * If an invalid string is specified, the program will terminate with an IllegalValue exception.
 * Possible values are: "EXACT", "LONGEST", and "DEFAULT".
 *
 * @param [in] str A pointer to a string representation of the desired `CPINameRouteType`.
 *
 * @return The `NameRouteType` matching the specified string.
 *
 * Example:
 * @code
 * {
 *     CPINameRouteType type = cpiNameRouteType_FromString("EXACT");
 * }
 * @endcode
 *
 * @see cpiNameRouteType_ToString
 */
CPINameRouteType cpiNameRouteType_FromString(const char *str);
#endif // libccnx_cpi_NameRouteType_h
