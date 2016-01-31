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
 *
 * Implements the RtaCommandCreateProtocolStack object which signals to RTA Framework to open a new connection
 * with the given configuration.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <ccnx/transport/transport_rta/commands/rta_CommandCreateProtocolStack.h>

struct rta_command_createprotocolstack {
    int stackId;
    CCNxStackConfig *config;
};

static void
_rtaCommandCreateProtocolStack_Destroy(RtaCommandCreateProtocolStack **openConnectionPtr)
{
    RtaCommandCreateProtocolStack *openConnection = *openConnectionPtr;

    if (openConnection->config) {
        ccnxStackConfig_Release(&openConnection->config);
    }
}

parcObject_ExtendPARCObject(RtaCommandCreateProtocolStack, _rtaCommandCreateProtocolStack_Destroy,
                            NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(rtaCommandCreateProtocolStack, RtaCommandCreateProtocolStack);

parcObject_ImplementRelease(rtaCommandCreateProtocolStack, RtaCommandCreateProtocolStack);

RtaCommandCreateProtocolStack *
rtaCommandCreateProtocolStack_Create(int stackId, CCNxStackConfig *config)
{
    RtaCommandCreateProtocolStack *createStack = parcObject_CreateInstance(RtaCommandCreateProtocolStack);
    createStack->stackId = stackId;
    createStack->config = ccnxStackConfig_Copy(config);
    return createStack;
}

const char *
rtaCommandCreateProtocolStack_AssessValidity(const RtaCommandCreateProtocolStack *instance)
{
    char *result = NULL;

    if (instance != NULL) {
        if (ccnxStackConfig_IsValid(instance->config)) {
            result = NULL;
        } else {
            result = "CCNxStackConfig instance is invalid";
        }
    } else {
        result = "Instance cannot be NULL";
    }

    return result;
}

bool
rtaCommandCreateProtocolStack_IsValid(const RtaCommandCreateProtocolStack *instance)
{
    const char *assessment = rtaCommandCreateProtocolStack_AssessValidity(instance);
    return assessment == NULL;
}

void
rtaCommandCreateProtocolStack_AssertValid(const RtaCommandCreateProtocolStack *instance)
{
    const char *assessment = rtaCommandCreateProtocolStack_AssessValidity(instance);
    trapIllegalValueIf(assessment != NULL, "%s", assessment);
}

int
rtaCommandCreateProtocolStack_GetStackId(const RtaCommandCreateProtocolStack *createStack)
{
    assertNotNull(createStack, "Parameter createStack must be non-null");
    return createStack->stackId;
}

CCNxStackConfig *
rtaCommandCreateProtocolStack_GetStackConfig(const RtaCommandCreateProtocolStack *createStack)
{
    return createStack->config;
}

PARCJSON *
rtaCommandCreateProtocolStack_GetConfig(const RtaCommandCreateProtocolStack *createStack)
{
    assertNotNull(createStack, "Parameter createStack must be non-null");
    return ccnxStackConfig_GetJson(createStack->config);
}
