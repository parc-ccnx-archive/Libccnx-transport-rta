/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author <#gscott#>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <transport_Stack.h>

struct TransportStack {
    int topFD;
    int bottomFD;
};

static void
_transportStack_Finalize(TransportStack **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a TransportStack pointer.");
    TransportStack *instance = *instancePtr;

    transportStack_OptionalAssertValid(instance);

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(transportStack, TransportStack);

parcObject_ImplementRelease(transportStack, TransportStack);

parcObject_ExtendPARCObject(TransportStack, _transportStack_Finalize, transportStack_Copy, transportStack_ToString, transportStack_Equals, transportStack_Compare, transportStack_HashCode, transportStack_ToJSON);


void
transportStack_AssertValid(const TransportStack *instance)
{
    assertTrue(transportStack_IsValid(instance),
               "TransportStack is not valid.");
}


TransportStack *
transportStack_Create(void)
{
    TransportStack *result = parcObject_CreateInstance(TransportStack);

    return result;
}

int
transportStack_Compare(const TransportStack *instance, const TransportStack *other)
{
    int result = 0;

    return result;
}

TransportStack *
transportStack_Copy(const TransportStack *original)
{
    TransportStack *result = NULL;

    return result;
}

void
transportStack_Display(const TransportStack *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "TransportStack@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
transportStack_Equals(const TransportStack *x, const TransportStack *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        /* perform instance specific equality tests here. */
    }

    return result;
}

PARCHashCode
transportStack_HashCode(const TransportStack *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
transportStack_IsValid(const TransportStack *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
transportStack_ToJSON(const TransportStack *instance)
{
    PARCJSON *result = parcJSON_Create();

    return result;
}

char *
transportStack_ToString(const TransportStack *instance)
{
    char *result = parcMemory_Format("TransportStack@%p\n", instance);

    return result;
}
