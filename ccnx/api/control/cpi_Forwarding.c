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
#include <ccnx/api/control/cpi_Forwarding.h>
#include <LongBow/runtime.h>

#include "cpi_private.h"

static const char *cpiRegister = "REGISTER";
static const char *cpiUnregister = "UNREGISTER";
static const char *cpiRouteList = "ROUTE_LIST";

PARCJSON *
cpiForwarding_CreateAddRouteRequest(const CPIRouteEntry *route)
{
    PARCJSON *routeAsJSON = cpiRouteEntry_ToJson(route);
    PARCJSON *result = cpi_CreateRequest(cpiRegister, routeAsJSON);
    parcJSON_Release(&routeAsJSON);

    return result;
}

PARCJSON *
cpiForwarding_CreateRemoveRouteRequest(const CPIRouteEntry *route)
{
    PARCJSON *routeAsJSON = cpiRouteEntry_ToJson(route);
    PARCJSON *result = cpi_CreateRequest(cpiUnregister, routeAsJSON);
    parcJSON_Release(&routeAsJSON);

    return result;
}

PARCJSON *
cpiForwarding_AddRouteToSelf(const CCNxName *prefix)
{
    CPIRouteEntry *route = cpiRouteEntry_CreateRouteToSelf(prefix);
    PARCJSON *result = cpiForwarding_AddRoute(route);
    cpiRouteEntry_Destroy(&route);
    return result;
}

PARCJSON *
cpiForwarding_RemoveRouteToSelf(const CCNxName *prefix)
{
    CPIRouteEntry *route = cpiRouteEntry_CreateRouteToSelf(prefix);
    PARCJSON *result = cpiForwarding_RemoveRoute(route);
    cpiRouteEntry_Destroy(&route);
    return result;
}

PARCJSON *
cpiForwarding_AddRoute(const CPIRouteEntry *route)
{
    PARCJSON *operation = cpiRouteEntry_ToJson(route);
    PARCJSON *result = cpi_CreateRequest(cpiRegister, operation);
    parcJSON_Release(&operation);

    return result;
}

PARCJSON *
cpiForwarding_RemoveRoute(const CPIRouteEntry *route)
{
    PARCJSON *operation = cpiRouteEntry_ToJson(route);
    PARCJSON *result = cpi_CreateRequest(cpiUnregister, operation);
    parcJSON_Release(&operation);

    return result;
}

CPIRouteEntry *
cpiForwarding_RouteFromControlMessage(CCNxControl *control)
{
    assertNotNull(control, "Parameter control must be non-null");
    PARCJSON *json = ccnxControl_GetJson(control);

    PARCJSONPair *routeOpPair = cpi_ParseRequest(json);
    PARCJSON *routeJson = parcJSONValue_GetJSON(parcJSONPair_GetValue(routeOpPair));

    CPIRouteEntry *route = cpiRouteEntry_FromJson(routeJson);

    return route;
}

const char *
cpiForwarding_AddRouteJsonTag()
{
    return cpiRegister;
}

const char *
cpiForwarding_RemoveRouteJsonTag()
{
    return cpiUnregister;
}

const char *
cpiForwarding_RouteListJsonTag()
{
    return cpiRouteList;
}

PARCJSON *
cpiForwarding_CreateRouteListRequest()
{
    PARCJSON *json = parcJSON_Create();
    PARCJSON *result = cpi_CreateRequest(cpiRouteList, json);
    parcJSON_Release(&json);

    return result;
}

CPIRouteEntryList *
cpiForwarding_RouteListFromControlMessage(CCNxControl *control)
{
    PARCJSON *json = ccnxControl_GetJson(control);
    PARCJSONValue *value = parcJSON_GetValueByName(json, cpiRequest_GetJsonTag());
    if (value == NULL) {
        value = parcJSON_GetValueByName(json, cpiResponse_GetJsonTag());
    }
    PARCJSON *innerJson = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(innerJson, cpiRouteList);
    PARCJSON *operation = parcJSONValue_GetJSON(value);
    return cpiRouteEntryList_FromJson(operation);
}
