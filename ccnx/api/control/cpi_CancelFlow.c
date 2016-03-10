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
#include <string.h>

#include <LongBow/runtime.h>

#include <ccnx/api/control/controlPlaneInterface.h>
#include <ccnx/api/control/cpi_CancelFlow.h>
#include <parc/algol/parc_Memory.h>

static const char *cpiCancelFlow = "CPI_CANCEL_FLOW";
static const char *cpiFlowName = "FLOW_NAME";

PARCJSON *
cpiCancelFlow_CreateRequest(const CCNxName *name)
{
    PARCJSON *operation = parcJSON_Create();

    char *uri = ccnxName_ToString(name);
    parcJSON_AddString(operation, cpiFlowName, uri);
    parcMemory_Deallocate((void **) &uri);

    PARCJSON *result = cpi_CreateRequest(cpiCancelFlow, operation);
    parcJSON_Release(&operation);

    return result;
}

PARCJSON *
cpiCancelFlow_Create(const CCNxName *name)
{
    PARCJSON *operation = parcJSON_Create();

    char *uri = ccnxName_ToString(name);
    parcJSON_AddString(operation, cpiFlowName, uri);
    parcMemory_Deallocate((void **) &uri);

    PARCJSON *result = cpi_CreateRequest(cpiCancelFlow, operation);
    parcJSON_Release(&operation);

    return result;
}

CCNxName *
cpiCancelFlow_GetFlowName(const PARCJSON *controlMessage)
{
    assertNotNull(controlMessage, "Parameter controlMessage must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(controlMessage, cpiRequest_GetJsonTag());
    assertNotNull(value, "only support getting the name from a Request at the moment, not from an ack/nack.");
    PARCJSON *inner_json = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(inner_json, cpiCancelFlow_CancelFlowJsonTag());
    assertNotNull(value, "Missing JSON tag in control message: %s", cpiCancelFlow_CancelFlowJsonTag());
    inner_json = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(inner_json, cpiFlowName);
    assertNotNull(value, "Missing JSON tag in control message: %s", cpiFlowName);
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    const char *uri = parcBuffer_Overlay(sBuf, 0);

    CCNxName *name = ccnxName_CreateFromCString(uri);

    return name;
}

CCNxName *
cpiCancelFlow_NameFromControlMessage(CCNxControl *control)
{
    assertNotNull(control, "Parameter control must be non-null");
    return cpiCancelFlow_GetFlowName(ccnxControl_GetJson(control));
}

bool
cpiCancelFlow_SuccessFromResponse(CCNxControl *control)
{
    trapNotImplemented("cpiCancelFlow_SuccessFromResponse");
}

const char *
cpiCancelFlow_CancelFlowJsonTag(void)
{
    return cpiCancelFlow;
}
