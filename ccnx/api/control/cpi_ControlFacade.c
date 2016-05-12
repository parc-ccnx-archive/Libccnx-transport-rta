/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Marc Mosko, Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>

#include <ccnx/api/control/cpi_ControlFacade.h>
#include <ccnx/common/codec/schema_v1/ccnxCodecSchemaV1_TlvDictionary.h>

#include <ccnx/common/ccnx_Name.h>

static const char _NotificationIndicator[] = "notificationWrapper";
static const char _NotificationPayload[] = "notificationPayload";


// ===========================================================================================================


CCNxControl *
ccnxControlFacade_CreateCPI(PARCJSON *ccnx_json)
{
    assertNotNull(ccnx_json, "Parameter ccnx_json must be non-null");

    CCNxTlvDictionary *dictionary = ccnxCodecSchemaV1TlvDictionary_CreateControl();

    ccnxTlvDictionary_PutJson(dictionary, CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD, ccnx_json);

    return dictionary;
}

CCNxControl *ccnxControlFacade_CreateNotification(PARCJSON *payload) {
    assertNotNull(payload, "Parameter ccnx_json must be non-null");
   
    CCNxTlvDictionary *dictionary = ccnxCodecSchemaV1TlvDictionary_CreateControl();
   
    // Create a new JSON object that indicates that this is a notification. Wrap it around
    // the supplied JSON object.
   
    PARCJSON *notificationWrapper = parcJSON_Create();
    parcJSON_AddBoolean(notificationWrapper, _NotificationIndicator, true);
    parcJSON_AddObject(notificationWrapper,  _NotificationPayload, payload);
    ccnxTlvDictionary_PutJson(dictionary, CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD, notificationWrapper);
    parcJSON_Release(&notificationWrapper);
   
    return dictionary;
}

PARCJSON *
ccnxControlFacade_GetJson(const CCNxTlvDictionary *controlDictionary)
{
    ccnxControlFacade_AssertValid(controlDictionary);
    PARCJSON *controlJSON = ccnxTlvDictionary_GetJson(controlDictionary, CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD);

    if (ccnxControlFacade_IsNotification(controlDictionary)) {
        PARCJSONValue *wrappedJSON = parcJSON_GetValueByName(controlJSON, _NotificationPayload);
        controlJSON = parcJSONValue_GetJSON(wrappedJSON);
    }
   
    return controlJSON;
}

bool
ccnxControlFacade_IsCPI(const CCNxTlvDictionary *controlDictionary)
{
    bool result = false;
    ccnxControlFacade_AssertValid(controlDictionary);
   
    result = ccnxTlvDictionary_IsControl(controlDictionary);
   
    PARCJSON *controlJSON = ccnxTlvDictionary_GetJson(controlDictionary, CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD);
    if (controlJSON != NULL) {
        if (parcJSON_GetValueByName(controlJSON, _NotificationIndicator) != NULL) {
            // this is a notification
            result = false;
        }
    }
    return result;
}

bool
ccnxControlFacade_IsNotification(const CCNxTlvDictionary *controlDictionary)
{
    bool result = false;
   
    ccnxControlFacade_AssertValid(controlDictionary);
       
    PARCJSON *controlJSON = ccnxTlvDictionary_GetJson(controlDictionary, CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD);
    if (controlJSON != NULL && (parcJSON_GetValueByName(controlJSON, _NotificationIndicator) != NULL)) {
        result = true;
    }
    return result;
}

void
ccnxControlFacade_Display(const CCNxTlvDictionary *contentDictionary, int indentation)
{
    ccnxTlvDictionary_Display(contentDictionary, indentation);
}

char *
ccnxControlFacade_ToString(const CCNxTlvDictionary *contentDictionary)
{
    char *string;
    char *jsonString = NULL;

    PARCJSON *json = ccnxControlFacade_GetJson(contentDictionary);
    if (json != NULL) {
        jsonString = parcJSON_ToString(json);
    }

    int failure = asprintf(&string, "CCNxControl { isCPI=%s, isNotification=%s, JSON=\"%s\"}",
                           ccnxControlFacade_IsCPI(contentDictionary) ? "true" : "false",
                           ccnxControlFacade_IsNotification(contentDictionary) ? "true" : "false",
                           jsonString != NULL ? jsonString : "NULL");


    if (jsonString) {
        parcMemory_Deallocate((void **) &jsonString);
    }

    assertTrue(failure > -1, "Error asprintf");

    char *result = parcMemory_StringDuplicate(string, strlen(string));
    free(string);

    return result;
}

void
ccnxControlFacade_AssertValid(const CCNxTlvDictionary *controlDictionary)
{
    assertNotNull(controlDictionary, "Parameter must be a non-null CCNxControlFacade pointer");

   
    assertTrue(ccnxTlvDictionary_IsValueJson(controlDictionary,
                                             CCNxCodecSchemaV1TlvDictionary_MessageFastArray_PAYLOAD), "Does not have JSON payload");
    assertTrue(ccnxTlvDictionary_IsControl(controlDictionary), "Does not have type set");
}
