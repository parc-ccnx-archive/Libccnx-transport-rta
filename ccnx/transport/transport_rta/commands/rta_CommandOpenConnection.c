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
 * Implements the RtaCommandOpenConnection object which signals to RTA Framework to open a new connection
 * with the given configuration.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <ccnx/transport/transport_rta/commands/rta_CommandOpenConnection.h>

struct rta_command_openconnection {
    int stackId;
    int apiNotifierFd;
    int transportNotifierFd;
    PARCJSON *config;
};

// ======= Private API

static void
_rtaCommandOpenConnection_Destroy(RtaCommandOpenConnection **openConnectionPtr)
{
    RtaCommandOpenConnection *openConnection = *openConnectionPtr;
    if (openConnection->config != NULL) {
        parcJSON_Release(&openConnection->config);
    }
}

parcObject_ExtendPARCObject(RtaCommandOpenConnection, _rtaCommandOpenConnection_Destroy,
                            NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(rtaCommandOpenConnection, RtaCommandOpenConnection);

parcObject_ImplementRelease(rtaCommandOpenConnection, RtaCommandOpenConnection);

// ======= Public API

RtaCommandOpenConnection *
rtaCommandOpenConnection_Create(int stackId, int apiNotifierFd, int transportNotifierFd, const PARCJSON *config)
{
    RtaCommandOpenConnection *openConnection = parcObject_CreateInstance(RtaCommandOpenConnection);
    openConnection->stackId = stackId;
    openConnection->apiNotifierFd = apiNotifierFd;
    openConnection->transportNotifierFd = transportNotifierFd;
    openConnection->config = parcJSON_Copy(config);
    return openConnection;
}

int
rtaCommandOpenConnection_GetApiNotifierFd(const RtaCommandOpenConnection *openConnection)
{
    assertNotNull(openConnection, "Parameter openConnection must be non-null");
    return openConnection->apiNotifierFd;
}

int
rtaCommandOpenConnection_GetStackId(const RtaCommandOpenConnection *openConnection)
{
    assertNotNull(openConnection, "Parameter openConnection must be non-null");
    return openConnection->stackId;
}

int
rtaCommandOpenConnection_GetTransportNotifierFd(const RtaCommandOpenConnection *openConnection)
{
    assertNotNull(openConnection, "Parameter openConnection must be non-null");
    return openConnection->transportNotifierFd;
}

PARCJSON *
rtaCommandOpenConnection_GetConfig(const RtaCommandOpenConnection *openConnection)
{
    assertNotNull(openConnection, "Parameter openConnection must be non-null");
    return openConnection->config;
}
