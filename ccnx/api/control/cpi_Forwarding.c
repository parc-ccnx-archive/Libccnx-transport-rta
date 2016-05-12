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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
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
