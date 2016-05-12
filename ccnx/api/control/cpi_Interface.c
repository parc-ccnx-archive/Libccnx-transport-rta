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
#include <stdlib.h>
#include <string.h>

#include <ccnx/api/control/cpi_AddressList.h>
#include <ccnx/api/control/cpi_Interface.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/algol/parc_Object.h>

#include <LongBow/runtime.h>

struct cpi_interface {
    char *name;
    unsigned interfaceIndex;
    bool loopback;
    bool supportMulticast;
    unsigned mtu;

    CPIAddressList *addressList;
};

char *
cpiInterface_ToString(const CPIInterface *interface)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();

    parcBufferComposer_Format(composer, "%3u %10s %1s%1s %8u ",
                              interface->interfaceIndex,
                              interface->name,
                              interface->loopback ? "l" : " ",
                              interface->supportMulticast ? "m" : " ",
                              interface->mtu);

    for (size_t i = 0; i < cpiAddressList_Length(interface->addressList); i++) {
        cpiAddress_BuildString(cpiAddressList_GetItem(interface->addressList, i), composer);
        if (i < (cpiAddressList_Length(interface->addressList) - 1)) {
            parcBufferComposer_PutStrings(composer, "\n", NULL);
        }
    }

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *result = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);
    parcBufferComposer_Release(&composer);
    return result;
}

CPIInterface *
cpiInterface_Create(const char *name, unsigned interfaceIndex, bool loopback, bool supportMulticast, unsigned mtu)
{
    CPIInterface *iface = parcMemory_AllocateAndClear(sizeof(CPIInterface));

    assertNotNull(iface, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIInterface));
    iface->name = parcMemory_StringDuplicate(name, 64);
    iface->interfaceIndex = interfaceIndex;
    iface->loopback = loopback;
    iface->supportMulticast = supportMulticast;
    iface->mtu = mtu;
    iface->addressList = cpiAddressList_Create();

    return iface;
}

void
cpiInterface_Destroy(CPIInterface **interfacePtr)
{
    assertNotNull(interfacePtr, "Parameter must be non-null double pointer");
    assertNotNull(*interfacePtr, "Parameter must dereference to non-null pointer");

    CPIInterface *iface = *interfacePtr;
    parcMemory_Deallocate((void **) &iface->name);
    cpiAddressList_Destroy(&iface->addressList);
    parcMemory_Deallocate((void **) &iface);
    interfacePtr = NULL;
}

void
cpiInterface_AddAddress(CPIInterface *iface, CPIAddress *address)
{
    assertNotNull(iface, "Parameter iface must be non-null");

    size_t length = cpiAddressList_Length(iface->addressList);
    for (size_t i = 0; i < length; i++) {
        const CPIAddress *a = cpiAddressList_GetItem(iface->addressList, i);
        if (cpiAddress_Equals(a, address)) {
            return;
        }
    }

    cpiAddressList_Append(iface->addressList, address);
}

const CPIAddressList *
cpiInterface_GetAddresses(const CPIInterface *iface)
{
    assertNotNull(iface, "Parameter iface must be non-null");
    return iface->addressList;
}

unsigned
cpiInterface_GetInterfaceIndex(const CPIInterface *iface)
{
    assertNotNull(iface, "Parameter iface must be non-null");
    return iface->interfaceIndex;
}

bool
cpiInterface_NameEquals(const CPIInterface *iface, const char *name)
{
    assertNotNull(iface, "Parameter iface must be non-null");

    if (strcasecmp(iface->name, name) == 0) {
        return true;
    }
    return false;
}

bool
cpiInterface_Equals(const CPIInterface *a, const CPIInterface *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }

    if (a == NULL || b == NULL) {
        return false;
    }

    if (a->interfaceIndex == b->interfaceIndex) {
        if (a->loopback == b->loopback) {
            if (a->supportMulticast == b->supportMulticast) {
                if (a->mtu == b->mtu) {
                    if (strcasecmp(a->name, b->name) == 0) {
                        if (cpiAddressList_Equals(a->addressList, b->addressList)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

static const char cpi_Iface[]      = "Interface";
static const char cpi_IfName[]     = "Name";
static const char cpi_IFIDX[]      = "Index";
static const char cpi_IsLoopback[] = "Loopback";
static const char cpi_Multicast[]  = "Multicast";
static const char cpi_MTU[]        = "MTU";

static const char cpi_True[]  = "true";
static const char cpi_False[] = "false";
static const char cpi_Addrs[] = "Addrs";

PARCJSON *
cpiInterface_ToJson(CPIInterface *iface)
{
    assertNotNull(iface, "Parameter must be non-null");

    PARCJSON *inner_json = parcJSON_Create();

    parcJSON_AddString(inner_json, cpi_IfName, iface->name);
    parcJSON_AddInteger(inner_json, cpi_IFIDX, iface->interfaceIndex);
    parcJSON_AddString(inner_json, cpi_IsLoopback, iface->loopback ? cpi_True : cpi_False);
    parcJSON_AddString(inner_json, cpi_Multicast, iface->supportMulticast ? cpi_True : cpi_False);
    parcJSON_AddInteger(inner_json, cpi_MTU, iface->mtu);

    PARCJSONArray *addrsArray = cpiAddressList_ToJson(iface->addressList);
    parcJSON_AddArray(inner_json, cpi_Addrs, addrsArray);
    parcJSONArray_Release(&addrsArray);

    PARCJSON *outter_json = parcJSON_Create();
    parcJSON_AddObject(outter_json, cpi_Iface, inner_json);
    parcJSON_Release(&inner_json);

    return outter_json;
}

CPIInterface *
cpiInterface_FromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");


    PARCJSONValue *value = parcJSON_GetValueByName(json, cpi_Iface);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpi_Iface,
                  parcJSON_ToString(json));
    PARCJSON *ifaceJson = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(ifaceJson, cpi_IfName);
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    const char *name = parcBuffer_Overlay(sBuf, 0);
    value = parcJSON_GetValueByName(ifaceJson, cpi_IFIDX);
    unsigned ifidx = (unsigned)parcJSONValue_GetInteger(value);
    value = parcJSON_GetValueByName(ifaceJson, cpi_IsLoopback);
    sBuf = parcJSONValue_GetString(value);
    const char *loopStr = parcBuffer_Overlay(sBuf, 0);
    value = parcJSON_GetValueByName(ifaceJson, cpi_Multicast);
    sBuf = parcJSONValue_GetString(value);
    const char *mcastStr = parcBuffer_Overlay(sBuf, 0);
    value = parcJSON_GetValueByName(ifaceJson, cpi_MTU);
    unsigned mtu = (unsigned)parcJSONValue_GetInteger(value);
    value = parcJSON_GetValueByName(ifaceJson, cpi_Addrs);
    PARCJSONArray *addrsJson = parcJSONValue_GetArray(value);

    bool isLoopback = (strcasecmp(loopStr, cpi_True) == 0);
    bool supportsMulticast = (strcasecmp(mcastStr, cpi_True) == 0);

    CPIInterface *iface = cpiInterface_Create(name, ifidx, isLoopback, supportsMulticast, mtu);

    CPIAddressList *addrs = cpiAddressList_CreateFromJson(addrsJson);
    for (size_t i = 0; i < cpiAddressList_Length(addrs); i++) {
        const CPIAddress *addr = cpiAddressList_GetItem(addrs, i);
        cpiInterface_AddAddress(iface, cpiAddress_Copy(addr));
    }

    cpiAddressList_Destroy(&addrs);
    return iface;
}

const char *
cpiInterface_GetName(const CPIInterface *iface)
{
    assertNotNull(iface, "Parameter iface must be non-null");
    return iface->name;
}

unsigned
cpiInterface_GetMTU(const CPIInterface *iface)
{
    assertNotNull(iface, "Parameter iface must be non-null");
    return iface->mtu;
}

