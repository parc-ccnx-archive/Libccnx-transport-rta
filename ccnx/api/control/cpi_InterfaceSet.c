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

#include <ccnx/api/control/cpi_InterfaceSet.h>

#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_Memory.h>

#include <LongBow/runtime.h>

struct cpi_interface_set {
    PARCArrayList *listOfInterfaces;
};

static void
_destroyInterface(void **ifaceVoidPtr)
{
    cpiInterface_Destroy((CPIInterface **) ifaceVoidPtr);
}

CPIInterfaceSet *
cpiInterfaceSet_Create(void)
{
    CPIInterfaceSet *set = parcMemory_AllocateAndClear(sizeof(CPIInterfaceSet));
    assertNotNull(set, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIInterfaceSet));
    set->listOfInterfaces = parcArrayList_Create(_destroyInterface);
    return set;
}

void
cpiInterfaceSet_Destroy(CPIInterfaceSet **setPtr)
{
    assertNotNull(setPtr, "Parameter must be non-null double pointer");
    assertNotNull(*setPtr, "Parameter must dereference to non-null pointer");

    CPIInterfaceSet *set = *setPtr;
    parcArrayList_Destroy(&set->listOfInterfaces);
    parcMemory_Deallocate((void **) &set);
    *setPtr = NULL;
}

bool
cpiInterfaceSet_Add(CPIInterfaceSet *set, CPIInterface *iface)
{
    assertNotNull(set, "Parameter set must be non-null");
    assertNotNull(iface, "Parameter iface must be non-null");

    unsigned ifaceIndex = cpiInterface_GetInterfaceIndex(iface);
    size_t length = parcArrayList_Size(set->listOfInterfaces);
    for (size_t i = 0; i < length; i++) {
        CPIInterface *listEntry = (CPIInterface *) parcArrayList_Get(set->listOfInterfaces, i);
        unsigned entryInterfaceIndex = cpiInterface_GetInterfaceIndex(listEntry);
        if (entryInterfaceIndex == ifaceIndex) {
            return false;
        }
    }

    parcArrayList_Add(set->listOfInterfaces, (PARCObject *) iface);
    return true;
}

size_t
cpiInterfaceSet_Length(const CPIInterfaceSet *set)
{
    assertNotNull(set, "Parameter set must be non-null");
    return parcArrayList_Size(set->listOfInterfaces);
}

CPIInterface *
cpiInterfaceSet_GetByOrdinalIndex(CPIInterfaceSet *set, size_t ordinalIndex)
{
    assertNotNull(set, "Parameter set must be non-null");
    return (CPIInterface *) parcArrayList_Get(set->listOfInterfaces, ordinalIndex);
}

CPIInterface *
cpiInterfaceSet_GetByInterfaceIndex(const CPIInterfaceSet *set, unsigned interfaceIndex)
{
    size_t length = parcArrayList_Size(set->listOfInterfaces);
    for (size_t i = 0; i < length; i++) {
        CPIInterface *listEntry = (CPIInterface *) parcArrayList_Get(set->listOfInterfaces, i);
        unsigned entryInterfaceIndex = cpiInterface_GetInterfaceIndex(listEntry);
        if (entryInterfaceIndex == interfaceIndex) {
            return listEntry;
        }
    }
    return NULL;
}

/**
 * Uses the system name (e.g. "en0")
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *
cpiInterfaceSet_GetByName(CPIInterfaceSet *set, const char *name)
{
    size_t length = parcArrayList_Size(set->listOfInterfaces);
    for (size_t i = 0; i < length; i++) {
        CPIInterface *listEntry = (CPIInterface *) parcArrayList_Get(set->listOfInterfaces, i);
        if (cpiInterface_NameEquals(listEntry, name)) {
            return listEntry;
        }
    }
    return NULL;
}

bool
cpiInterfaceSet_Equals(const CPIInterfaceSet *a, const CPIInterfaceSet *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }

    if (a == NULL || b == NULL) {
        return false;
    }

    size_t length_a = parcArrayList_Size(a->listOfInterfaces);
    size_t length_b = parcArrayList_Size(b->listOfInterfaces);

    if (length_a == length_b) {
        for (size_t i = 0; i < length_a; i++) {
            CPIInterface *iface_a = (CPIInterface *) parcArrayList_Get(a->listOfInterfaces, i);

            // the set is unique by interface id, so if it exists in set b, it
            // exists there by interface id
            CPIInterface *iface_b = cpiInterfaceSet_GetByInterfaceIndex(b, cpiInterface_GetInterfaceIndex(iface_a));
            if (!cpiInterface_Equals(iface_b, iface_b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

const char cpi_InterfaceList[] = "Interfaces";

CPIInterfaceSet *
cpiInterfaceSet_FromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(json, cpi_InterfaceList);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpi_InterfaceList,
                  parcJSON_ToString(json));
    PARCJSONArray *ifaceSetJson = parcJSONValue_GetArray(value);

    CPIInterfaceSet *set = cpiInterfaceSet_Create();

    size_t length = parcJSONArray_GetLength(ifaceSetJson);
    for (size_t i = 0; i < length; i++) {
        value = parcJSONArray_GetValue(ifaceSetJson, i);
        PARCJSON *ifaceJson = parcJSONValue_GetJSON(value);
        CPIInterface *iface = cpiInterface_FromJson(ifaceJson);
        cpiInterfaceSet_Add(set, iface);
    }

    return set;
}

PARCJSON *
cpiInterfaceSet_ToJson(CPIInterfaceSet *set)
{
    assertNotNull(set, "Parameter must be non-null");

    PARCJSONArray *interfaceList = parcJSONArray_Create();

    size_t length = parcArrayList_Size(set->listOfInterfaces);
    for (size_t i = 0; i < length; i++) {
        CPIInterface *iface = (CPIInterface *) parcArrayList_Get(set->listOfInterfaces, i);
        PARCJSON *json = cpiInterface_ToJson(iface);
        PARCJSONValue *value = parcJSONValue_CreateFromJSON(json);
        parcJSON_Release(&json);
        parcJSONArray_AddValue(interfaceList, value);
        parcJSONValue_Release(&value);
    }

    PARCJSON *result = parcJSON_Create();
    parcJSON_AddArray(result, cpi_InterfaceList, interfaceList);
    parcJSONArray_Release(&interfaceList);

    return result;
}
