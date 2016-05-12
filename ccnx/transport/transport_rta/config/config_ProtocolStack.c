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

#include "config_ProtocolStack.h"

#include <ccnx/transport/transport_rta/core/components.h>

#include <LongBow/runtime.h>

static const char param_STACK[] = "STACK";
static const char param_COMPONENTS[] = "COMPONENTS";

/*
 * Call with the names of each component, terminated by a NULL, for example:
 *
 * <code>
 * ccnxStackConfig_AppendComponents(stackConfig, apiConnector_GetName(), vegasFlowController_GetName(),
 * tlvCodec_GetName(), localForwarder_GetName(), NULL);
 * </code>
 *
 * Generates:
 *
 * { "STACK" : { "COMPONENTS" : [ name1, name2, ... ] }
 */
CCNxStackConfig *
protocolStack_ComponentsConfigArgs(CCNxStackConfig *stackConfig, ...)
{
    PARCArrayList *list = parcArrayList_Create(NULL);

    va_list ap;
    const char *componentName;
    va_start(ap, stackConfig);

    while ((componentName = va_arg(ap, const char *)) != NULL) {
        parcArrayList_Add(list, (char *) componentName);
    }

    va_end(ap);

    stackConfig = protocolStack_ComponentsConfigArrayList(stackConfig, list);
    parcArrayList_Destroy(&list);

    return stackConfig;
}

/**
 * Same as <code>protocolStack_ComponentsConfigArgs</code>, except uses
 * an ArrayList of <code>const char *</code> component names.
 */
CCNxStackConfig *
protocolStack_ComponentsConfigArrayList(CCNxStackConfig *stackConfig, const PARCArrayList *listOfComponentNames)
{
    PARCJSON *stackJson = parcJSON_Create();
    PARCJSONArray *arrayJson = parcJSONArray_Create();

    for (int i = 0; i < parcArrayList_Size(listOfComponentNames); i++) {
        char *componentName = parcArrayList_Get(listOfComponentNames, i);
        PARCJSONValue *value = parcJSONValue_CreateFromCString(componentName);
        parcJSONArray_AddValue(arrayJson, value);
        parcJSONValue_Release(&value);
    }

    parcJSON_AddArray(stackJson, param_COMPONENTS, arrayJson);
    parcJSONArray_Release(&arrayJson);

    PARCJSONValue *value = parcJSONValue_CreateFromJSON(stackJson);
    parcJSON_Release(&stackJson);

    CCNxStackConfig *result = ccnxStackConfig_Add(stackConfig, param_STACK, value);
    parcJSONValue_Release(&value);
    return result;
}

const char *
protocolStack_GetName(void)
{
    return param_STACK;
}

/**
 * Parse the protocol stack json to extract an array list of the component names
 */
PARCArrayList *
protocolStack_GetComponentNameArray(PARCJSON *protocolStackJson)
{
    // created with NULL destroyer because we're putting in const char *
    PARCArrayList *arraylist = parcArrayList_Create_Capacity(NULL, NULL, 16);

    PARCJSONValue *value = parcJSON_GetValueByName(protocolStackJson, param_STACK);
    assertNotNull(value, "Cannot have null %s key in json", param_STACK);
    PARCJSON *stackJson = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(stackJson, param_COMPONENTS);
    assertNotNull(value, "Cannot have null %s key in json", param_COMPONENTS);
    assertTrue(parcJSONValue_IsArray(value), "key %s not type ARRAY", param_COMPONENTS);
    PARCJSONArray *componentsJson = parcJSONValue_GetArray(value);

    size_t length = parcJSONArray_GetLength(componentsJson);

    for (size_t i = 0; i < length; i++) {
        value = parcJSONArray_GetValue(componentsJson, i);
        PARCBuffer *sBuf = parcJSONValue_GetString(value);
        parcArrayList_Add(arraylist, parcBuffer_Overlay(sBuf, 0));
    }
    return arraylist;
}
