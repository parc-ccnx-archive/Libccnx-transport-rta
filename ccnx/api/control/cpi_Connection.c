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
#include <string.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/algol/parc_JSON.h>

#include <ccnx/api/control/cpi_Connection.h>
#include <ccnx/api/control/cpi_InterfaceGeneric.h>

#define SOURCE_INDEX 0
#define DESTINATION_INDEX 1

const static char cpiIFIDX[]    = "IFIDX";
const static char cpiSRCADDR[]  = "SRC";
const static char cpiDSTADDR[]  = "DST";
const static char cpiCONNTYPE[] = "CONNTYPE";
const static char cpiSTATE[]    = "STATE";


struct cpi_connection {
    CPIInterfaceGeneric *generic;
    CPIConnectionType tunnelType;
};

struct connection_type_string_s {
    CPIConnectionType type;
    const char *str;
} connectionTypeStrings[] = {
    { .type = cpiConnection_UDP,       .str = "UDP"   },
    { .type = cpiConnection_TCP,       .str = "TCP"   },
    { .type = cpiConnection_GRE,       .str = "GRE"   },
    { .type = cpiConnection_MULTICAST, .str = "MCAST" },
    { .type = cpiConnection_L2,        .str = "L2"    },
    { .type = 0,                       .str = NULL    },
};


static void
_cpiConnection_Destroy(CPIConnection **iptunPtr)
{
    assertNotNull(iptunPtr, "Parameter must be non-null double pointer");
    assertNotNull(*iptunPtr, "Parameter must dereference to non-null pointer");

    CPIConnection *iptun = *iptunPtr;
    cpiInterfaceGeneric_Destroy(&iptun->generic);
}

parcObject_ExtendPARCObject(CPIConnection, _cpiConnection_Destroy, cpiConnection_Copy, cpiConnection_ToString, cpiConnection_Equals, NULL, NULL, cpiConnection_ToJson);

parcObject_ImplementAcquire(cpiConnection, CPIConnection);

parcObject_ImplementRelease(cpiConnection, CPIConnection);

static PARCBufferComposer *
cpiConnection_BuildString(const CPIConnection *connection, PARCBufferComposer *composer)
{
    cpiInterfaceGeneric_BuildString(connection->generic, composer);
    cpiConnectionType_BuildString(connection->tunnelType, composer);

    return composer;
}

char *
cpiConnection_ToString(const CPIConnection *connection)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    cpiConnection_BuildString(connection, composer);

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *result = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);
    parcBufferComposer_Release(&composer);
    return result;
}

PARCBufferComposer *
cpiConnectionType_BuildString(CPIConnectionType type, PARCBufferComposer *composer)
{
    return parcBufferComposer_PutStrings(composer, " ", cpiConnectionType_ToString(type), NULL);
}

const char *
cpiConnectionType_ToString(CPIConnectionType type)
{
    for (int i = 0; connectionTypeStrings[i].str != NULL; i++) {
        if (connectionTypeStrings[i].type == type) {
            return connectionTypeStrings[i].str;
        }
    }
    trapIllegalValue(type, "Unknown type: %d", type);
}

CPIConnectionType
cpiConnectionType_FromString(const char *str)
{
    for (int i = 0; connectionTypeStrings[i].str != NULL; i++) {
        if (strcasecmp(connectionTypeStrings[i].str, str) == 0) {
            return connectionTypeStrings[i].type;
        }
    }
    trapIllegalValue(type, "Unknown type: %s", str);
}

CPIConnection *
cpiConnection_Create(unsigned ifidx, CPIAddress *source, CPIAddress *destination, CPIConnectionType tunnelType)
{
    assertNotNull(source, "Parameter source must be non-null");
    assertNotNull(destination, "Parameter destination must be non-null");

    CPIConnection *iptun = parcObject_CreateInstance(CPIConnection);
    assertNotNull(iptun, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIConnection));

    CPIAddressList *addrlist = cpiAddressList_Create();
    cpiAddressList_Append(addrlist, source);
    cpiAddressList_Append(addrlist, destination);

    iptun->generic = cpiInterfaceGeneric_Create(ifidx, addrlist);
    iptun->tunnelType = tunnelType;
    return iptun;
}

CPIConnection *
cpiConnection_Copy(const CPIConnection *original)
{
    assertNotNull(original, "Parameter original must be non-null");
    CPIConnection *iptun = parcObject_CreateInstance(CPIConnection);
    assertNotNull(iptun, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIConnection));
    iptun->generic = cpiInterfaceGeneric_Copy(original->generic);
    iptun->tunnelType = original->tunnelType;
    return iptun;
}

void
cpiConnection_SetState(CPIConnection *iptun, CPIInterfaceStateType state)
{
    assertNotNull(iptun, "Parameter must be non-null");
    cpiInterfaceGeneric_SetState(iptun->generic, state);
}

unsigned
cpiConnection_GetIndex(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");
    return cpiInterfaceGeneric_GetIndex(iptun->generic);
}

const CPIAddress *
cpiConnection_GetSourceAddress(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");
    const CPIAddressList *addrs = cpiInterfaceGeneric_GetAddresses(iptun->generic);
    return cpiAddressList_GetItem(addrs, SOURCE_INDEX);
}

const CPIAddress *
cpiConnection_GetDestinationAddress(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");
    const CPIAddressList *addrs = cpiInterfaceGeneric_GetAddresses(iptun->generic);
    return cpiAddressList_GetItem(addrs, DESTINATION_INDEX);
}

CPIConnectionType
cpiConnection_GetConnectionType(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");
    return iptun->tunnelType;
}

CPIInterfaceStateType
cpiConnection_GetState(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");
    return cpiInterfaceGeneric_GetState(iptun->generic);
}

bool
cpiConnection_Equals(const CPIConnection *a, const CPIConnection *b)
{
    assertNotNull(a, "Parameter a must be non-null");
    assertNotNull(b, "Parameter b must be non-null");

    if (a->tunnelType == b->tunnelType) {
        return cpiInterfaceGeneric_Equals(a->generic, b->generic);
    }
    return false;
}

static const char *cpiConnection = "Connection";

PARCJSON *
cpiConnection_ToJson(const CPIConnection *iptun)
{
    assertNotNull(iptun, "Parameter must be non-null");

    PARCJSON *inner_json = parcJSON_Create();

    parcJSON_AddInteger(inner_json, cpiIFIDX, cpiConnection_GetIndex(iptun));

    if (cpiConnection_GetState(iptun) != CPI_IFACE_UNKNOWN) {
        parcJSON_AddString(inner_json, cpiSTATE, cpiInterfaceStateType_ToString(cpiConnection_GetState(iptun)));
    }

    parcJSON_AddString(inner_json, cpiCONNTYPE, cpiConnectionType_ToString(cpiConnection_GetConnectionType(iptun)));

    PARCJSON *json = cpiAddress_ToJson(cpiConnection_GetSourceAddress(iptun));
    parcJSON_AddObject(inner_json, cpiSRCADDR, json);
    parcJSON_Release(&json);

    json = cpiAddress_ToJson(cpiConnection_GetDestinationAddress(iptun));
    parcJSON_AddObject(inner_json, cpiDSTADDR, json);
    parcJSON_Release(&json);

    PARCJSON *outter_json = parcJSON_Create();
    parcJSON_AddObject(outter_json, cpiConnection, inner_json);
    parcJSON_Release(&inner_json);

    return outter_json;
}

CPIConnection *
cpiConnection_CreateFromJson(PARCJSON *json)
{
    assertNotNull(json, "Parameter must be non-null");

    PARCJSONValue *value = parcJSON_GetValueByName(json, cpiConnection);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  cpiConnection,
                  parcJSON_ToString(json));

    PARCJSON *connectionJson = parcJSONValue_GetJSON(value);

    value = parcJSON_GetValueByName(connectionJson, cpiIFIDX);
    assertNotNull(value,
                  "Could not find key %s: %s", cpiIFIDX, parcJSON_ToString(json));
    assertTrue(parcJSONValue_IsNumber(value),
               "%s is not a number: %s",
               cpiIFIDX,
               parcJSON_ToString(json));
    PARCJSONValue *ifidx_value = value;

    value = parcJSON_GetValueByName(connectionJson, cpiCONNTYPE);
    assertNotNull(value,
                  "Could not find key %s: %s", cpiCONNTYPE, parcJSON_ToString(json));
    assertTrue(parcJSONValue_IsString(value),
               "%s is not a number: %s",
               cpiCONNTYPE,
               parcJSON_ToString(json));
    PARCJSONValue *tuntype_value = value;

    value = parcJSON_GetValueByName(connectionJson, cpiSRCADDR);
    assertNotNull(value,
                  "Could not find key %s: %s", cpiSRCADDR, parcJSON_ToString(json));
    assertTrue(parcJSONValue_IsJSON(value),
               "%s is not an array: %s",
               cpiSRCADDR,
               parcJSON_ToString(json));
    PARCJSONValue *srcaddr_value = value;

    value = parcJSON_GetValueByName(connectionJson, cpiDSTADDR);
    assertNotNull(value,
                  "Could not find key %s: %s", cpiDSTADDR, parcJSON_ToString(json));
    PARCJSONValue *dstaddr_value = value;
    assertTrue(parcJSONValue_IsJSON(value),
               "%s is not an array: %s",
               cpiDSTADDR,
               parcJSON_ToString(json));

    unsigned ifidx = (unsigned)parcJSONValue_GetInteger(ifidx_value);
    CPIAddress *srcaddr =
        cpiAddress_CreateFromJson(parcJSONValue_GetJSON(srcaddr_value));
    CPIAddress *dstaddr =
        cpiAddress_CreateFromJson(parcJSONValue_GetJSON(dstaddr_value));
    PARCBuffer *sbuf = parcJSONValue_GetString(tuntype_value);
    CPIConnectionType tunnelType =
        cpiConnectionType_FromString(parcBuffer_Overlay(sbuf, 0));

    CPIConnection *iptun = cpiConnection_Create(ifidx, srcaddr, dstaddr, tunnelType);

    PARCJSONValue *state_value = parcJSON_GetValueByName(connectionJson, cpiSTATE);
    if (state_value != NULL) {
        sbuf = parcJSONValue_GetString(state_value);
        cpiConnection_SetState(iptun, cpiInterfaceStateType_FromString(parcBuffer_Overlay(sbuf, 0)));
    }

    return iptun;
}
