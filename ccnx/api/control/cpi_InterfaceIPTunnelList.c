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

#include <ccnx/api/control/cpi_InterfaceIPTunnelList.h>
#include <LongBow/runtime.h>

struct cpi_interface_iptunnel_list {
    PARCArrayList *listOfTunnels;
};

/**
 * PARCArrayList entry destroyer
 */
static void
_arrayDestroyer(void **voidPtr)
{
    CPIInterfaceIPTunnel **entryPtr = (CPIInterfaceIPTunnel **) voidPtr;
    cpiInterfaceIPTunnel_Release(entryPtr);
}

CPIInterfaceIPTunnelList *
cpiInterfaceIPTunnelList_Create(void)
{
    CPIInterfaceIPTunnelList *list = parcMemory_AllocateAndClear(sizeof(CPIInterfaceIPTunnelList));
    assertNotNull(list, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIInterfaceIPTunnelList));
    list->listOfTunnels = parcArrayList_Create(_arrayDestroyer);
    return list;
}

void
cpiInterfaceIPTunnelList_Destroy(CPIInterfaceIPTunnelList **listPtr)
{
    assertNotNull(listPtr, "Parameter must be non-null double pointer");
    assertNotNull(*listPtr, "Parameter must dereference to non-null pointer");
    CPIInterfaceIPTunnelList *list = *listPtr;
    parcArrayList_Destroy(&list->listOfTunnels);
    parcMemory_Deallocate((void **) &list);
    *listPtr = NULL;
}

void
cpiInterfaceIPTunnelList_Append(CPIInterfaceIPTunnelList *list, CPIInterfaceIPTunnel *entry)
{
    assertNotNull(list, "Parameter list must be non-null");
    assertNotNull(entry, "Parameter entry must be non-null");

    parcArrayList_Add(list->listOfTunnels, (PARCObject *) entry);
}

size_t
cpiInterfaceIPTunnelList_Length(const CPIInterfaceIPTunnelList *list)
{
    assertNotNull(list, "Parameter list must be non-null");
    return parcArrayList_Size(list->listOfTunnels);
}

CPIInterfaceIPTunnel *
cpiInterfaceIPTunnelList_Get(CPIInterfaceIPTunnelList *list, size_t index)
{
    assertNotNull(list, "Parameter list must be non-null");
    CPIInterfaceIPTunnel *original = (CPIInterfaceIPTunnel *) parcArrayList_Get(list->listOfTunnels, index);
    return cpiInterfaceIPTunnel_Copy(original);
}

bool
cpiInterfaceIPTunnelList_Equals(const CPIInterfaceIPTunnelList *a, const CPIInterfaceIPTunnelList *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }

    if (parcArrayList_Size(a->listOfTunnels) == parcArrayList_Size(b->listOfTunnels)) {
        size_t length = parcArrayList_Size(a->listOfTunnels);
        for (size_t i = 0; i < length; i++) {
            CPIInterfaceIPTunnel *tunnel_a = (CPIInterfaceIPTunnel *) parcArrayList_Get(a->listOfTunnels, i);
            CPIInterfaceIPTunnel *tunnel_b = (CPIInterfaceIPTunnel *) parcArrayList_Get(b->listOfTunnels, i);
            if (!cpiInterfaceIPTunnel_Equals(tunnel_a, tunnel_b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

const char cpi_InterfaceIPTunnelList[] = "TunnelList";

PARCJSON *
cpiInterfaceIPTunnelList_ToJson(const CPIInterfaceIPTunnelList *list)
{
    assertNotNull(list, "Parameter must be non-null");

    PARCJSONArray *tunnelList = parcJSONArray_Create();

    size_t length = parcArrayList_Size(list->listOfTunnels);
    for (size_t i = 0; i < length; i++) {
        CPIInterfaceIPTunnel *tunnel = (CPIInterfaceIPTunnel *) parcArrayList_Get(list->listOfTunnels, i);
        PARCJSON *json = cpiInterfaceIPTunnel_ToJson(tunnel);
        PARCJSONValue *value = parcJSONValue_CreateFromJSON(json);
        parcJSON_Release(&json);
        parcJSONArray_AddValue(tunnelList, value);
        parcJSONValue_Release(&value);
    }

    PARCJSON *result = parcJSON_Create();
    parcJSON_AddArray(result, cpi_InterfaceIPTunnelList, tunnelList);
    parcJSONArray_Release(&tunnelList);

    return result;
}

CPIInterfaceIPTunnelList *
cpiInterfaceIPTunnelList_FromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(json, cpi_InterfaceIPTunnelList);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpi_InterfaceIPTunnelList,
                  parcJSON_ToString(json));
    PARCJSONArray *tunnelListJson = parcJSONValue_GetArray(value);

    CPIInterfaceIPTunnelList *list = cpiInterfaceIPTunnelList_Create();

    size_t length = parcJSONArray_GetLength(tunnelListJson);
    for (size_t i = 0; i < length; i++) {
        value = parcJSONArray_GetValue(tunnelListJson, i);
        CPIInterfaceIPTunnel *tunnel =
            cpiInterfaceIPTunnel_CreateFromJson(parcJSONValue_GetJSON(value));

        cpiInterfaceIPTunnelList_Append(list, tunnel);
    }

    return list;
}
