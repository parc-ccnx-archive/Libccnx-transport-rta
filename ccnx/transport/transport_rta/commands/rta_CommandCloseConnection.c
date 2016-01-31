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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 *
 * Implements the RtaCommandCloseConnection object.  Only has to pass one argument, the apiSocket number,
 * to identify which connection to close.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <ccnx/transport/transport_rta/commands/rta_CommandCloseConnection.h>

struct rta_command_closeconnection {
    int apiNotifierFd;
};

parcObject_ExtendPARCObject(RtaCommandCloseConnection, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(rtaCommandCloseConnection, RtaCommandCloseConnection);

parcObject_ImplementRelease(rtaCommandCloseConnection, RtaCommandCloseConnection);

RtaCommandCloseConnection *
rtaCommandCloseConnection_Create(int apiNotifierFd)
{
    RtaCommandCloseConnection *closeConnection = parcObject_CreateInstance(RtaCommandCloseConnection);
    closeConnection->apiNotifierFd = apiNotifierFd;
    return closeConnection;
}

int
rtaCommandCloseConnection_GetApiNotifierFd(const RtaCommandCloseConnection *closeConnection)
{
    assertNotNull(closeConnection, "Parameter closeConnection must be non-null");
    return closeConnection->apiNotifierFd;
}
