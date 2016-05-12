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
 * @file rta_ConnectionTable.h
 * @brief Data structure of connections.  It is managed by rtaFramework.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef Libccnx_rta_ConnectionTable_h
#define Libccnx_rta_ConnectionTable_h

#include "rta_Connection.h"

struct rta_connection_table;
typedef struct rta_connection_table RtaConnectionTable;

typedef void (TableFreeFunc)(RtaConnection **connection);

/**
 * Create a connection table of the given size.  Whenever a
 * connection is removed, the freefunc is called.  Be sure that
 * does not in turn call back in to the connection table.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnectionTable *rtaConnectionTable_Create(size_t elements, TableFreeFunc *freefunc);

/**
 * Destroy the connection table, and it will call freefunc()
 * on each connection in the table.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaConnectionTable_Destroy(RtaConnectionTable **tablePtr);

/**
 * Add a connetion to the table.  Stores the reference provided (does not copy).
 * Returns 0 on success, -1 on error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int rtaConnectionTable_AddConnection(RtaConnectionTable *table, RtaConnection *connection);

/**
 * Lookup a connection.
 * Returns NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *rtaConnectionTable_GetByApiFd(RtaConnectionTable *table, int api_fd);

/**
 * Lookup a connection.
 * Returns NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *rtaConnectionTable_GetByTransportFd(RtaConnectionTable *table, int transport_fd);

/**
 * Remove a connection from the table, calling freefunc() on it.
 * Returns 0 on success, -1 if not found (or error)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int rtaConnectionTable_Remove(RtaConnectionTable *table, RtaConnection *connection);

/**
 * Remove all connections in a given stack_id, calling freefunc() on it.
 * Returns 0 on success, -1 if not found (or error)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int rtaConnectionTable_RemoveByStack(RtaConnectionTable *table, int stack_id);
#endif
