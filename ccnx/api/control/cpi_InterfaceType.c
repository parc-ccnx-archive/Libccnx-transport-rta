/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <LongBow/runtime.h>

typedef enum {
    CPI_IFACE_LOOPBACK = 1,
    CPI_IFACE_ETHERNET = 2,
    CPI_IFACE_LOCALAPP = 3,
    CPI_IFACE_TUNNEL = 4,
    CPI_IFACE_GROUP = 5
} CPIInterfaceType;

typedef enum {
    CPI_IFACE_UNKNOWN = 0,
    CPI_IFACE_UP = 1,
    CPI_IFACE_DOWN = 2
} CPIInterfaceStateType;

struct cpi_interface_types_string {
    CPIInterfaceType type;
    const char *str;
} interfaceTypesArray[] = {
    { .type = CPI_IFACE_LOOPBACK, .str = "LOOPBACK" },
    { .type = CPI_IFACE_ETHERNET, .str = "ETHERNET" },
    { .type = CPI_IFACE_LOCALAPP, .str = "LOCALAPP" },
    { .type = CPI_IFACE_TUNNEL,   .str = "TUNNEL"   },
    { .type = CPI_IFACE_GROUP,    .str = "GROUP"    },
    { .type = 0,                  .str = NULL       }
};

struct cpi_interface_state_types_string {
    CPIInterfaceStateType type;
    const char *str;
} interfaceStateTypesArray[] = {
    { .type = CPI_IFACE_UNKNOWN, .str = "UNKNOWN" },
    { .type = CPI_IFACE_UP,      .str = "UP"      },
    { .type = CPI_IFACE_DOWN,    .str = "DOWN"    },
    { .type = 0,                 .str = NULL      }
};

const char *
cpiInterfaceType_ToString(CPIInterfaceType type)
{
    for (int i = 0; interfaceTypesArray[i].str != NULL; i++) {
        if (interfaceTypesArray[i].type == type) {
            return interfaceTypesArray[i].str;
        }
    }
    trapIllegalValue(type, "Unknown type: %d", type);
}

CPIInterfaceType
cpiInterfaceType_FromString(const char *str)
{
    for (int i = 0; interfaceTypesArray[i].str != NULL; i++) {
        if (strcasecmp(interfaceTypesArray[i].str, str) == 0) {
            return interfaceTypesArray[i].type;
        }
    }
    // use a LongBow trap here, instead of an assertion followed by abort().
    assertTrue(0, "Unknown stirng: %s", str);
    abort();
}

const char *
cpiInterfaceStateType_ToString(CPIInterfaceStateType type)
{
    for (int i = 0; interfaceStateTypesArray[i].str != NULL; i++) {
        if (interfaceStateTypesArray[i].type == type) {
            return interfaceStateTypesArray[i].str;
        }
    }
    trapIllegalValue(type, "Unknown type: %d", type);
}

CPIInterfaceStateType
cpiInterfaceStateType_FromString(const char *str)
{
    for (int i = 0; interfaceStateTypesArray[i].str != NULL; i++) {
        if (strcasecmp(interfaceStateTypesArray[i].str, str) == 0) {
            return interfaceStateTypesArray[i].type;
        }
    }
    assertTrue(0, "Unknown stirng: %s", str);
    abort();
}
