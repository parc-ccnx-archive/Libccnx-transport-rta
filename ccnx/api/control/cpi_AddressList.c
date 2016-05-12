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

#include <LongBow/runtime.h>

#include <ccnx/api/control/cpi_AddressList.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_Buffer.h>

struct cpi_addresslist {
    PARCArrayList *listOfCPIAddress;
};

static void
_cpiAddressList_FreeAddress(void **addressVoidPtr)
{
    CPIAddress **addressPtr = (CPIAddress **) addressVoidPtr;
    cpiAddress_Destroy(addressPtr);
}

CPIAddressList *
cpiAddressList_Create()
{
    CPIAddressList *list = parcMemory_AllocateAndClear(sizeof(CPIAddressList));
    assertNotNull(list, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIAddressList));
    list->listOfCPIAddress = parcArrayList_Create(_cpiAddressList_FreeAddress);
    assertNotNull(list->listOfCPIAddress, "Got null from parcArrayList_Create");

    return list;
}

void
cpiAddressList_Destroy(CPIAddressList **addressListPtr)
{
    assertNotNull(addressListPtr, "Parameter must be non-null double pointer");
    assertNotNull(*addressListPtr, "Parameter must dereference to non-null pointer");
    CPIAddressList *list = *addressListPtr;

    parcArrayList_Destroy(&list->listOfCPIAddress);
    parcMemory_Deallocate((void **) &list);
    *addressListPtr = NULL;
}

CPIAddressList *
cpiAddressList_Append(CPIAddressList *list, CPIAddress *address)
{
    assertNotNull(list, "Parameter list must be non-null");
    assertNotNull(address, "Parameter address must be non-null");

    parcArrayList_Add(list->listOfCPIAddress, (PARCObject *)address);
    return list;
}

CPIAddressList *
cpiAddressList_Copy(const CPIAddressList *original)
{
    assertNotNull(original, "Parameter must be non-null");

    CPIAddressList *copy = cpiAddressList_Create();
    for (int i = 0; i < parcArrayList_Size(original->listOfCPIAddress); i++) {
        CPIAddress *address = (CPIAddress *)parcArrayList_Get(original->listOfCPIAddress, i);
        parcArrayList_Add(copy->listOfCPIAddress, (PARCObject *)cpiAddress_Copy(address));
    }

    return copy;
}

bool
cpiAddressList_Equals(const CPIAddressList *a, const CPIAddressList *b)
{
    assertNotNull(a, "Parameter a must be non-null");
    assertNotNull(b, "Parameter b must be non-null");

    if (a == b) {
        return true;
    }

    if (parcArrayList_Size(a->listOfCPIAddress) != parcArrayList_Size(b->listOfCPIAddress)) {
        return false;
    }

    for (size_t i = 0; i < parcArrayList_Size(a->listOfCPIAddress); i++) {
        const CPIAddress *addr_a = (CPIAddress *)parcArrayList_Get(a->listOfCPIAddress, i);
        const CPIAddress *addr_b = (CPIAddress *)parcArrayList_Get(b->listOfCPIAddress, i);
        if (!cpiAddress_Equals(addr_a, addr_b)) {
            return false;
        }
    }
    return true;
}

size_t
cpiAddressList_Length(const CPIAddressList *list)
{
    assertNotNull(list, "Parameter must be non-null");
    return parcArrayList_Size(list->listOfCPIAddress);
}

const CPIAddress *
cpiAddressList_GetItem(const CPIAddressList *list, size_t item)
{
    assertNotNull(list, "Parameter must be non-null");
    assertTrue(item < cpiAddressList_Length(list), "Asked for item %zu beyond end of list %zu", item, cpiAddressList_Length(list));

    return (CPIAddress *)parcArrayList_Get(list->listOfCPIAddress, item);
}

/**
 * Returns a JSON array of the addresses
 *
 *   { [ {addr0}, {addr1}, ..., {addrN} ] }
 *
 * @param <#param1#>
 * @return A JSON array, even if array empty
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSONArray *
cpiAddressList_ToJson(const CPIAddressList *list)
{
    assertNotNull(list, "Parameter must be non-null");
    PARCJSONArray *array = parcJSONArray_Create();

    for (size_t i = 0; i < cpiAddressList_Length(list); i++) {
        const CPIAddress *addr = cpiAddressList_GetItem(list, i);
        PARCJSON *json = cpiAddress_ToJson(addr);
        PARCJSONValue *value = parcJSONValue_CreateFromJSON(json);
        parcJSON_Release(&json);
        parcJSONArray_AddValue(array, value);
        parcJSONValue_Release(&value);
    }

    return array;
}

/**
 * Creates an address list based on a JSON array
 *
 *   { [ {addr0}, {addr1}, ..., {addrN} ] }
 *
 * @param <#param1#>
 * @return An allocated address list.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */

CPIAddressList *
cpiAddressList_CreateFromJson(PARCJSONArray *array)
{
    assertNotNull(array, "Parameter must be non-null");
    CPIAddressList *list = cpiAddressList_Create();

    for (size_t i = 0; i < parcJSONArray_GetLength(array); i++) {
        PARCJSONValue *value = parcJSONArray_GetValue(array, i);
        PARCJSON *addrjson = parcJSONValue_GetJSON(value);
        CPIAddress *addr = cpiAddress_CreateFromJson(addrjson);
        cpiAddressList_Append(list, addr);
    }

    return list;
}

char *
cpiAddressList_ToString(const CPIAddressList *list)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();

    for (size_t i = 0; i < cpiAddressList_Length(list); i++) {
        char *addressString = cpiAddress_ToString(cpiAddressList_GetItem(list, i));
        parcBufferComposer_PutString(composer, addressString);
        if (i < (cpiAddressList_Length(list) - 1)) {
            parcBufferComposer_PutString(composer, " ");
        }
        parcMemory_Deallocate((void **) &addressString);
    }

    PARCBuffer *buffer = parcBufferComposer_ProduceBuffer(composer);
    char *result = parcBuffer_ToString(buffer);
    parcBuffer_Release(&buffer);
    parcBufferComposer_Release(&composer);

    return result;
}
