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
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_ArrayList.h>

#include <ccnx/api/control/cpi_RouteEntryList.h>
#include <LongBow/runtime.h>

struct cpi_route_entry_list {
    PARCArrayList *listOfRouteEntries;
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
_cpiRouteEntryList_ArrayDestroyer(void **voidPtr)
{
    CPIRouteEntry **entryPtr = (CPIRouteEntry **) voidPtr;
    cpiRouteEntry_Destroy(entryPtr);
}

CPIRouteEntryList *
cpiRouteEntryList_Create()
{
    CPIRouteEntryList *list = parcMemory_AllocateAndClear(sizeof(CPIRouteEntryList));
    assertNotNull(list, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIRouteEntryList));
    list->listOfRouteEntries = parcArrayList_Create(_cpiRouteEntryList_ArrayDestroyer);
    return list;
}

void
cpiRouteEntryList_Destroy(CPIRouteEntryList **listPtr)
{
    assertNotNull(listPtr, "Parameter must be non-null double pointer");
    assertNotNull(*listPtr, "Parameter must dereference to non-null pointer");
    CPIRouteEntryList *list = *listPtr;
    parcArrayList_Destroy(&list->listOfRouteEntries);
    parcMemory_Deallocate((void **) &list);
    *listPtr = NULL;
}

void
cpiRouteEntryList_Append(CPIRouteEntryList *list, CPIRouteEntry *entry)
{
    assertNotNull(list, "Parameter list must be non-null");
    assertNotNull(entry, "Parameter entry must be non-null");

    parcArrayList_Add(list->listOfRouteEntries, (PARCObject *) entry);
}

size_t
cpiRouteEntryList_Length(const CPIRouteEntryList *list)
{
    assertNotNull(list, "Parameter list must be non-null");
    return parcArrayList_Size(list->listOfRouteEntries);
}

CPIRouteEntry *
cpiRouteEntryList_Get(CPIRouteEntryList *list, size_t index)
{
    assertNotNull(list, "Parameter list must be non-null");
    CPIRouteEntry *original = (CPIRouteEntry *) parcArrayList_Get(list->listOfRouteEntries, index);
    return cpiRouteEntry_Copy(original);
}


bool
cpiRouteEntryList_Equals(const CPIRouteEntryList *a, const CPIRouteEntryList *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }

    if (parcArrayList_Size(a->listOfRouteEntries) == parcArrayList_Size(b->listOfRouteEntries)) {
        size_t length = parcArrayList_Size(a->listOfRouteEntries);
        for (size_t i = 0; i < length; i++) {
            CPIRouteEntry *route_a = (CPIRouteEntry *) parcArrayList_Get(a->listOfRouteEntries, i);
            CPIRouteEntry *route_b = (CPIRouteEntry *) parcArrayList_Get(b->listOfRouteEntries, i);
            if (!cpiRouteEntry_Equals(route_a, route_b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}


const char cpi_RouteEntryList[] = "Routes";

PARCJSON *
cpiRouteEntryList_ToJson(const CPIRouteEntryList *list)
{
    assertNotNull(list, "Parameter must be non-null");

    PARCJSONArray *routeList = parcJSONArray_Create();

    size_t length = parcArrayList_Size(list->listOfRouteEntries);
    for (size_t i = 0; i < length; i++) {
        CPIRouteEntry *route = (CPIRouteEntry *) parcArrayList_Get(list->listOfRouteEntries, i);
        PARCJSON *json = cpiRouteEntry_ToJson(route);
        PARCJSONValue *value = parcJSONValue_CreateFromJSON(json);
        parcJSON_Release(&json);
        parcJSONArray_AddValue(routeList, value);
        parcJSONValue_Release(&value);
    }

    PARCJSON *result = parcJSON_Create();
    parcJSON_AddArray(result, cpi_RouteEntryList, routeList);
    parcJSONArray_Release(&routeList);

    return result;
}

CPIRouteEntryList *
cpiRouteEntryList_FromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(json, cpi_RouteEntryList);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpi_RouteEntryList,
                  parcJSON_ToString(json));
    PARCJSONArray *routeList = parcJSONValue_GetArray(value);

    CPIRouteEntryList *list = cpiRouteEntryList_Create();

    size_t length = parcJSONArray_GetLength(routeList);
    for (size_t i = 0; i < length; i++) {
        PARCJSONValue *value = parcJSONArray_GetValue(routeList, i);
        PARCJSON *routeJson = parcJSONValue_GetJSON(value);
        CPIRouteEntry *route = cpiRouteEntry_FromJson(routeJson);
        cpiRouteEntryList_Append(list, route);
    }

    return list;
}
