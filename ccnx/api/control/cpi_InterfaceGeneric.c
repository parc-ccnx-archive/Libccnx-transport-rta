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

#include <ccnx/api/control/cpi_InterfaceGeneric.h>
#include <LongBow/runtime.h>
#include <parc/algol/parc_Memory.h>
#include <string.h>

struct cpi_interface_generic {
    unsigned ifidx;
    CPIInterfaceStateType state;
    CPIAddressList *addresses;
};

CPIInterfaceGeneric *
cpiInterfaceGeneric_Create(unsigned ifidx, CPIAddressList *addresses)
{
    assertNotNull(addresses, "Parameter addresses must be non-null");

    CPIInterfaceGeneric *generic = parcMemory_AllocateAndClear(sizeof(CPIInterfaceGeneric));
    assertNotNull(generic, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIInterfaceGeneric));
    generic->ifidx = ifidx;
    generic->state = CPI_IFACE_UNKNOWN;
    generic->addresses = addresses;
    return generic;
}

CPIInterfaceGeneric *
cpiInterfaceGeneric_Copy(const CPIInterfaceGeneric *original)
{
    assertNotNull(original, "Parameter original must be non-null");

    CPIInterfaceGeneric *generic = parcMemory_AllocateAndClear(sizeof(CPIInterfaceGeneric));
    assertNotNull(generic, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(CPIInterfaceGeneric));
    generic->ifidx = original->ifidx;
    generic->state = original->state;
    generic->addresses = cpiAddressList_Copy(original->addresses);
    return generic;
}

void
cpiInterfaceGeneric_Destroy(CPIInterfaceGeneric **genericPtr)
{
    assertNotNull(genericPtr, "Parameter must be non-null double pointer");
    assertNotNull(*genericPtr, "Parameter must dereference to non-null pointer");

    CPIInterfaceGeneric *generic = *genericPtr;
    cpiAddressList_Destroy(&generic->addresses);
    parcMemory_Deallocate((void **) &generic);
    *genericPtr = NULL;
}

void
cpiInterfaceGeneric_SetState(CPIInterfaceGeneric *generic, CPIInterfaceStateType state)
{
    assertNotNull(generic, "Parameter must be non-null pointer");
    generic->state = state;
}

unsigned
cpiInterfaceGeneric_GetIndex(const CPIInterfaceGeneric *generic)
{
    assertNotNull(generic, "Parameter must be non-null pointer");
    return generic->ifidx;
}

const CPIAddressList *
cpiInterfaceGeneric_GetAddresses(const CPIInterfaceGeneric *generic)
{
    assertNotNull(generic, "Parameter must be non-null pointer");
    return generic->addresses;
}

CPIInterfaceStateType
cpiInterfaceGeneric_GetState(const CPIInterfaceGeneric *generic)
{
    assertNotNull(generic, "Parameter must be non-null pointer");
    return generic->state;
}

bool
cpiInterfaceGeneric_Equals(const CPIInterfaceGeneric *a, const CPIInterfaceGeneric *b)
{
    assertNotNull(a, "Parameter a must be non-null");
    assertNotNull(b, "Parameter b must be non-null");

    if (a == b) {
        return true;
    }

    if (a->ifidx == b->ifidx) {
        if (a->state == b->state) {
            if (cpiAddressList_Equals(a->addresses, b->addresses)) {
                return true;
            }
        }
    }
    return false;
}

PARCBufferComposer *
cpiInterfaceGeneric_BuildString(const CPIInterfaceGeneric *interface, PARCBufferComposer *composer)
{
    char *addressString = cpiAddressList_ToString(interface->addresses);
    parcBufferComposer_Format(composer, "%5d %4s %s",
                              interface->ifidx,
                              cpiInterfaceStateType_ToString(interface->state),
                              addressString
                              );
    parcMemory_Deallocate((void **) &addressString);
    return composer;
}
