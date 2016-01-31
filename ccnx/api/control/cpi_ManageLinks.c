/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <ccnx/api/control/controlPlaneInterface.h>
#include <ccnx/api/control/cpi_InterfaceIPTunnelList.h>
#include <ccnx/api/control/cpi_ManageLinks.h>
#include <LongBow/runtime.h>

#include "cpi_private.h"

static const char *cpiInterfaceList = "INTERFACE_LIST";
static const char *cpiCreateTunnel = "CREATE_TUNNEL";
static const char *cpiRemoveTunnel = "REMOVE_TUNNEL";
static const char *cpiConnectionList = "CONNECTION_LIST";

PARCJSON *
cpiLinks_CreateInterfaceListRequest(void)
{
    PARCJSON *json = parcJSON_Create();
    PARCJSON *result = cpi_CreateRequest(cpiInterfaceList, json);
    parcJSON_Release(&json);

    return result;
}

bool cpiLinks_IsInterfaceListResponse(CCNxControl *control);

CPIInterfaceSet *
cpiLinks_InterfacesFromControlMessage(CCNxControl *response)
{
    PARCJSON *json = ccnxControl_GetJson(response);
    PARCJSONValue *value = parcJSON_GetValueByName(json, cpiResponse_GetJsonTag());
    PARCJSON *inner_json = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(inner_json, cpiLinks_InterfaceListJsonTag());
    PARCJSON *operation = parcJSONValue_GetJSON(value);

    return cpiInterfaceSet_FromJson(operation);
}

CPIInterfaceIPTunnel *
cpiLinks_CreateIPTunnelFromControlMessage(CCNxControl *response)
{
    PARCJSON *json = ccnxControl_GetJson(response);
    PARCJSONValue *value = parcJSON_GetValueByName(json, cpiRequest_GetJsonTag());
    if (value == NULL) {
        value = parcJSON_GetValueByName(json, cpiResponse_GetJsonTag());
    }
    PARCJSON *inner_json = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(inner_json, cpiLinks_CreateTunnelJsonTag());
    PARCJSON *operation = parcJSONValue_GetJSON(value);
    return cpiInterfaceIPTunnel_CreateFromJson(operation);
}

PARCJSON *
cpiLinks_CreateConnectionListRequest()
{
    PARCJSON *json = parcJSON_Create();
    PARCJSON *result = cpi_CreateRequest(cpiConnectionList, json);
    parcJSON_Release(&json);

    return result;
}

CPIConnectionList *
cpiLinks_ConnectionListFromControlMessage(CCNxControl *response)
{
    PARCJSON *json = ccnxControl_GetJson(response);
    PARCJSONValue *value = parcJSON_GetValueByName(json, cpiRequest_GetJsonTag());
    if (value == NULL) {
        value = parcJSON_GetValueByName(json, cpiResponse_GetJsonTag());
    }
    PARCJSON *inner_json = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(inner_json, cpiLinks_ConnectionListJsonTag());
    PARCJSON *operation = parcJSONValue_GetJSON(value);
    return cpiConnectionList_FromJson(operation);
}

PARCJSON *
cpiLinks_CreateIPTunnel(const CPIInterfaceIPTunnel *iptun)
{
    PARCJSON *tunnelJson = cpiInterfaceIPTunnel_ToJson(iptun);
    PARCJSON *result = cpi_CreateRequest(cpiCreateTunnel, tunnelJson);
    parcJSON_Release(&tunnelJson);

    return result;
}

CCNxControl *
cpiLinks_SetInterfaceState(unsigned ifidx, CPIInterfaceStateType state)
{
    return NULL;
}

CCNxControl *
cpiLinks_RemoveInterface(unsigned ifidx)
{
    return NULL;
}

const char *
cpiLinks_InterfaceListJsonTag()
{
    return cpiInterfaceList;
}

const char *
cpiLinks_CreateTunnelJsonTag()
{
    return cpiCreateTunnel;
}

const char *
cpiLinks_RemoveTunnelJsonTag()
{
    return cpiRemoveTunnel;
}

const char *
cpiLinks_ConnectionListJsonTag()
{
    return cpiConnectionList;
}
