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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_ArrayList.h>

#include <ccnx/api/control/cpi_ConnectionList.h>
#include <LongBow/runtime.h>

struct cpi_connection_list {
    PARCArrayList *listOfConnections;
};

/**
 * PARCArrayList entry destroyer
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static void
_cpiConnectionList_ArrayDestroyer(void **voidPtr)
{
    CPIConnection **entryPtr = (CPIConnection **) voidPtr;
    cpiConnection_Release(entryPtr);
}

CPIConnectionList *
cpiConnectionList_Create()
{
    CPIConnectionList *list = parcMemory_AllocateAndClear(sizeof(CPIConnectionList));
    assertNotNull(list, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIConnectionList));
    list->listOfConnections = parcArrayList_Create(_cpiConnectionList_ArrayDestroyer);
    return list;
}

void
cpiConnectionList_Destroy(CPIConnectionList **listPtr)
{
    assertNotNull(listPtr, "Parameter must be non-null double pointer");
    assertNotNull(*listPtr, "Parameter must dereference to non-null pointer");
    CPIConnectionList *list = *listPtr;
    parcArrayList_Destroy(&list->listOfConnections);
    parcMemory_Deallocate((void **) &list);
    *listPtr = NULL;
}

void
cpiConnectionList_Append(CPIConnectionList *list, CPIConnection *entry)
{
    assertNotNull(list, "Parameter list must be non-null");
    assertNotNull(entry, "Parameter entry must be non-null");

    parcArrayList_Add(list->listOfConnections, entry);
}

size_t
cpiConnectionList_Length(const CPIConnectionList *list)
{
    assertNotNull(list, "Parameter list must be non-null");
    return parcArrayList_Size(list->listOfConnections);
}

CPIConnection *
cpiConnectionList_Get(CPIConnectionList *list, size_t index)
{
    assertNotNull(list, "Parameter list must be non-null");
    CPIConnection *original = (CPIConnection *) parcArrayList_Get(list->listOfConnections, index);
    return cpiConnection_Copy(original);
}

bool
cpiConnectionList_Equals(const CPIConnectionList *a, const CPIConnectionList *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }

    if (parcArrayList_Size(a->listOfConnections) == parcArrayList_Size(b->listOfConnections)) {
        size_t length = parcArrayList_Size(a->listOfConnections);
        for (size_t i = 0; i < length; i++) {
            CPIConnection *tunnel_a = (CPIConnection *) parcArrayList_Get(a->listOfConnections, i);
            CPIConnection *tunnel_b = (CPIConnection *) parcArrayList_Get(b->listOfConnections, i);
            if (!cpiConnection_Equals(tunnel_a, tunnel_b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

const char cpi_ConnectionList[] = "ConnectionList";

PARCJSON *
cpiConnectionList_ToJson(const CPIConnectionList *list)
{
    assertNotNull(list, "Parameter must be non-null");

    PARCJSONArray *inner_json = parcJSONArray_Create();

    size_t length = parcArrayList_Size(list->listOfConnections);
    for (size_t i = 0; i < length; i++) {
        CPIConnection *tunnel = (CPIConnection *) parcArrayList_Get(list->listOfConnections, i);
        PARCJSON *json = cpiConnection_ToJson(tunnel);
        PARCJSONValue *value = parcJSONValue_CreateFromJSON(json);
        parcJSON_Release(&json);
        parcJSONArray_AddValue(inner_json, value);
        parcJSONValue_Release(&value);
    }

    PARCJSON *outter_json = parcJSON_Create();
    parcJSON_AddArray(outter_json, cpi_ConnectionList, inner_json);
    parcJSONArray_Release(&inner_json);
    return outter_json;
}

CPIConnectionList *
cpiConnectionList_FromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(json, cpi_ConnectionList);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpi_ConnectionList,
                  parcJSON_ToString(json));
    PARCJSONArray *tunnelListJson = parcJSONValue_GetArray(value);

    CPIConnectionList *list = cpiConnectionList_Create();

    size_t length = parcJSONArray_GetLength(tunnelListJson);
    for (size_t i = 0; i < length; i++) {
        value = parcJSONArray_GetValue(tunnelListJson, i);
        PARCJSON *tunnelJson = parcJSONValue_GetJSON(value);
        CPIConnection *tunnel = cpiConnection_CreateFromJson(tunnelJson);
        cpiConnectionList_Append(list, tunnel);
    }

    return list;
}
