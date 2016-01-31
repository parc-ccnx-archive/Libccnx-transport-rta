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
 * This was written with PARCJSON, but that class is not really ready for prime time yet,
 * so there are some memory leaks in it that make determining if we have any memory leaks
 * difficult.  So, there may be some memory leaks... case 1029
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <ccnx/api/control/cpi_Listener.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_JSON.h>

#include <ccnx/api/control/controlPlaneInterface.h>
extern uint64_t cpi_GetNextSequenceNumber(void);

// JSON keys
static const char *KEY_IFNAME = "IFNAME";
static const char *KEY_SYMBOLIC = "SYMBOLIC";
static const char *KEY_ETHERTYPE = "ETHERTYPE";

static const char *KEY_IP_PROTOCOL = "IPROTO";
static const char *KEY_ADDR = "ADDR";

static const char *KEY_ADDLISTENER = "AddListener";
static const char *KEY_REMOVELISTENER = "RemoveListener";

typedef enum {
    CPIListenerMode_ETHER,
    CPIListenerMode_IP
} _CPIListenerMode;

struct cpi_listener {
    _CPIListenerMode mode;
    char *symbolic;

    char *interfaceName;
    uint16_t ethertype;

    CPIAddress *address;
    CPIInterfaceIPTunnelType type;
};

CPIListener *
cpiListener_CreateEther(const char *interfaceName, uint16_t ethertype, const char *symbolic)
{
    assertNotNull(interfaceName, "Parameter interfaceName must be non-null");
    assertNotNull(symbolic, "Parameter symbolic must be non-null");

    CPIListener *listener = parcMemory_AllocateAndClear(sizeof(CPIListener));
    if (listener) {
        listener->mode = CPIListenerMode_ETHER;
        listener->interfaceName = parcMemory_StringDuplicate(interfaceName, strlen(interfaceName));
        listener->symbolic = parcMemory_StringDuplicate(symbolic, strlen(symbolic));
        listener->ethertype = ethertype;
    }

    return listener;
}

CPIListener *
cpiListener_CreateIP(CPIInterfaceIPTunnelType type, CPIAddress *localAddress, const char *symbolic)
{
    assertNotNull(localAddress, "Parameter peerLinkAddress must be non-null");
    assertNotNull(symbolic, "Parameter symbolic must be non-null");

    CPIListener *listener = parcMemory_AllocateAndClear(sizeof(CPIListener));
    if (listener) {
        listener->mode = CPIListenerMode_IP;
        listener->type = type;
        listener->symbolic = parcMemory_StringDuplicate(symbolic, strlen(symbolic));
        listener->address = cpiAddress_Copy(localAddress);
    }

    return listener;
}

void
cpiListener_Release(CPIListener **listenerPtr)
{
    assertNotNull(listenerPtr, "Parameter listenerPtr must be non-null double pointer");
    assertNotNull(*listenerPtr, "Parameter listenerPtr dereference to non-null pointer");

    CPIListener *listener = *listenerPtr;

    if (listener->symbolic) {
        parcMemory_Deallocate((void **) &listener->symbolic);
    }

    if (listener->interfaceName) {
        parcMemory_Deallocate((void **) &listener->interfaceName);
    }

    if (listener->address) {
        cpiAddress_Destroy(&listener->address);
    }

    parcMemory_Deallocate((void **) &listener);
    *listenerPtr = NULL;
}

bool
cpiListener_Equals(const CPIListener *a, const CPIListener *b)
{
    if ((a == NULL && b == NULL) || a == b) {
        // both null or identically equal
        return true;
    }

    if (a == NULL || b == NULL) {
        // only one is null
        return false;
    }

    bool equals = false;
    if (a->mode == b->mode) {
        if (strcmp(a->symbolic, b->symbolic) == 0) {
            if (a->mode == CPIListenerMode_ETHER) {
                if (a->ethertype == b->ethertype) {
                    if (strcmp(a->interfaceName, b->interfaceName) == 0) {
                        equals = true;
                    }
                }
            } else {
                if (a->type == b->type) {
                    if (cpiAddress_Equals(a->address, b->address)) {
                        equals = true;
                    }
                }
            }
        }
    }

    return equals;
}

static void
_encodeEther(const CPIListener *listener, PARCJSON *json)
{
    // ------ Interface Name
    parcJSON_AddString(json, KEY_IFNAME, listener->interfaceName);

    // ------ EtherType
    parcJSON_AddInteger(json, KEY_ETHERTYPE, listener->ethertype);

    // ------ Symbolic Name
    parcJSON_AddString(json, KEY_SYMBOLIC, listener->symbolic);
}

static void
_encodeIP(const CPIListener *listener, PARCJSON *json)
{
    // ------ Tunnel Type
    const char *str = cpiInterfaceIPTunnel_TypeToString(listener->type);
    parcJSON_AddString(json, KEY_IP_PROTOCOL, str);

    // ------ Address
    PARCJSON *addressJson = cpiAddress_ToJson(listener->address);
    parcJSON_AddObject(json, KEY_ADDR, addressJson);
    parcJSON_Release(&addressJson);

    // ------ Symbolic Name
    parcJSON_AddString(json, KEY_SYMBOLIC, listener->symbolic);
}



static PARCJSON *
_cpiListener_ToJson(const CPIListener *listener)
{
    PARCJSON *json = parcJSON_Create();

    if (listener->mode == CPIListenerMode_ETHER) {
        _encodeEther(listener, json);
    } else {
        _encodeIP(listener, json);
    }

    return json;
}

/*
 * We want to create a JSON object that looks like this, where the operationName is either
 * AddListener or RemoveListener.
 *
 *  {
 *     "CPI_REQUEST" :
 *        {  "SEQUENCE" : <sequence number>,
 *           <operationName> : { "IFNAME" : "em1", "SYMBOLIC" : "conn0", "PEER_ADDR" : { "ADDRESSTYPE" : "LINK", "DATA" : "AQIDBAUG" }, "ETHERTYPE" : 2049 },
 *        }
 *  }
 */
static CCNxControl *
_cpiListener_CreateControlMessage(const CPIListener *listener, const char *operationName)
{
    // Figure out how to refactor this, case 1030

    PARCJSON *cpiRequest = parcJSON_Create();

    // --- add the seqnum

    uint64_t seqnum = cpi_GetNextSequenceNumber();
    parcJSON_AddInteger(cpiRequest, "SEQUENCE", (int) seqnum);

    // -- Add the operation

    PARCJSON *operation = _cpiListener_ToJson(listener);
    parcJSON_AddObject(cpiRequest, operationName, operation);
    parcJSON_Release(&operation);

    // -- Do the final encapusulation

    PARCJSON *final = parcJSON_Create();
    parcJSON_AddObject(final, cpiRequest_GetJsonTag(), cpiRequest);

    // -- Create the CPIControlMessage
    char *finalString = parcJSON_ToString(final);

    parcJSON_Release(&cpiRequest);
    parcJSON_Release(&final);

    PARCJSON *oldJson = parcJSON_ParseString(finalString);
    CCNxControl *result = ccnxControl_CreateCPIRequest(oldJson);
    parcJSON_Release(&oldJson);

    parcMemory_Deallocate((void **) &finalString);

    return result;
}

CCNxControl *
cpiListener_CreateAddMessage(const CPIListener *etherConn)
{
    assertNotNull(etherConn, "Parameter etherConn must be non-null");
    CCNxControl *control = _cpiListener_CreateControlMessage(etherConn, KEY_ADDLISTENER);
    return control;
}

CCNxControl *
cpiListener_CreateRemoveMessage(const CPIListener *etherConn)
{
    assertNotNull(etherConn, "Parameter etherConn must be non-null");
    CCNxControl *control = _cpiListener_CreateControlMessage(etherConn, KEY_REMOVELISTENER);
    return control;
}

static bool
_cpiListener_IsMessageType(const CCNxControl *control, const char *operationName)
{
    bool isOperation = false;
    if (ccnxControl_IsCPI(control)) {
        PARCJSON *oldJson = ccnxControl_GetJson(control);
        PARCJSONValue *value = parcJSON_GetValueByName(oldJson, cpiRequest_GetJsonTag());
        if (value != NULL) {
            // the second array element is the key we're looking for
            PARCJSON *innerJson = parcJSONValue_GetJSON(value);
            PARCJSONPair *opPair = parcJSON_GetPairByIndex(innerJson, 1);
            PARCBuffer *sBuf = parcJSONPair_GetName(opPair);
            const char *operation = parcBuffer_Overlay(sBuf, 0);
            if (operation && strcasecmp(operation, operationName) == 0) {
                isOperation = true;
            }
        }
    }

    return isOperation;
}

bool
cpiListener_IsAddMessage(const CCNxControl *control)
{
    assertNotNull(control, "Parameter control must be non-null");
    return _cpiListener_IsMessageType(control, KEY_ADDLISTENER);
}

bool
cpiListener_IsRemoveMessage(const CCNxControl *control)
{
    assertNotNull(control, "Parameter control must be non-null");
    return _cpiListener_IsMessageType(control, KEY_REMOVELISTENER);
}

static CPIListener *
_parseEther(PARCJSON *json)
{
    PARCJSONValue *value = parcJSON_GetValueByName(json, KEY_IFNAME);
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    const char *ifname = parcBuffer_Overlay(sBuf, 0);

    value = parcJSON_GetValueByName(json, KEY_SYMBOLIC);
    sBuf = parcJSONValue_GetString(value);
    const char *symbolic = parcBuffer_Overlay(sBuf, 0);

    value = parcJSON_GetValueByName(json, KEY_ETHERTYPE);
    int ethertype = (int) parcJSONValue_GetInteger(value);

    CPIListener *listener = cpiListener_CreateEther(ifname, (uint16_t) ethertype, symbolic);
    return listener;
}

static CPIListener *
_parseIP(PARCJSON *json)
{
    PARCJSONValue *value = parcJSON_GetValueByName(json, KEY_ADDR);
    assertNotNull(value,
                  "JSON key not found %s: %s",
                  KEY_ADDR,
                  parcJSON_ToString(json));
    PARCJSON *addrJson = parcJSONValue_GetJSON(value);

    CPIAddress *address = cpiAddress_CreateFromJson(addrJson);
    assertNotNull(address, "Failed to decode the address from %s", parcJSON_ToString(addrJson));

    value = parcJSON_GetValueByName(json, KEY_SYMBOLIC);
    PARCBuffer *sBuf = parcJSONValue_GetString(value);
    const char *symbolic = parcBuffer_Overlay(sBuf, 0);

    value = parcJSON_GetValueByName(json, KEY_IP_PROTOCOL);
    sBuf = parcJSONValue_GetString(value);
    const char *typeString = parcBuffer_Overlay(sBuf, 0);

    CPIInterfaceIPTunnelType type = cpiInterfaceIPTunnel_TypeFromString(typeString);

    CPIListener *listener = cpiListener_CreateIP(type, address, symbolic);
    cpiAddress_Destroy(&address);

    return listener;
}


CPIListener *
cpiListener_FromControl(const CCNxControl *control)
{
    assertNotNull(control, "Parameter control must be non-null");

    CPIListener *listener = NULL;

    if (ccnxControl_IsCPI(control)) {
        PARCJSON *oldJson = ccnxControl_GetJson(control);
        PARCJSONValue *value = parcJSON_GetValueByName(oldJson, cpiRequest_GetJsonTag());

        if (value != NULL) {
            PARCJSON *innerJson = parcJSONValue_GetJSON(value);
            // the second array element is the key we're looking for
            value = parcJSON_GetValueByName(innerJson, KEY_ADDLISTENER);
            if (value == NULL) {
                value = parcJSON_GetValueByName(innerJson, KEY_REMOVELISTENER);
            }
            if (value != NULL) {
                PARCJSON *operationJson = parcJSONValue_GetJSON(value);

                // if it has an interface name it's an ether
                value = parcJSON_GetValueByName(operationJson, KEY_IFNAME);
                if (value != NULL) {
                    listener = _parseEther(operationJson);
                } else {
                    listener = _parseIP(operationJson);
                }
            }
        }
    }

    return listener;
}

const char *
cpiListener_GetInterfaceName(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->interfaceName;
}

const char *
cpiListener_GetSymbolicName(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->symbolic;
}

CPIAddress *
cpiListener_GetAddress(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->address;
}

uint16_t
cpiListener_GetEtherType(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->ethertype;
}

bool
cpiListener_IsEtherEncap(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->mode == CPIListenerMode_ETHER;
}


bool
cpiListener_IsIPEncap(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return listener->mode == CPIListenerMode_IP;
}

bool
cpiListener_IsProtocolUdp(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return (listener->mode == CPIListenerMode_IP && listener->type == IPTUN_UDP);
}

bool
cpiListener_IsProtocolTcp(const CPIListener *listener)
{
    assertNotNull(listener, "Parameter listener must be non-null");
    return (listener->mode == CPIListenerMode_IP && listener->type == IPTUN_TCP);
}

