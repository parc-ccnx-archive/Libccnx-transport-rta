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
 * Implements the RtaCommandDestroyProtocolStack object which signals to RTA Framework to open a new connection
 * with the given configuration.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <ccnx/transport/transport_rta/commands/rta_CommandDestroyProtocolStack.h>

struct rta_command_destroyprotocolstack {
    int stackId;
};

parcObject_ExtendPARCObject(RtaCommandDestroyProtocolStack,
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(rtaCommandDestroyProtocolStack, RtaCommandDestroyProtocolStack);

parcObject_ImplementRelease(rtaCommandDestroyProtocolStack, RtaCommandDestroyProtocolStack);

// ======= Public API

RtaCommandDestroyProtocolStack *
rtaCommandDestroyProtocolStack_Create(int stackId)
{
    RtaCommandDestroyProtocolStack *createStack = parcObject_CreateInstance(RtaCommandDestroyProtocolStack);
    createStack->stackId = stackId;
    return createStack;
}


int
rtaCommandDestroyProtocolStack_GetStackId(const RtaCommandDestroyProtocolStack *destroyStack)
{
    assertNotNull(destroyStack, "Parameter destroyStack must be non-null");
    return destroyStack->stackId;
}
