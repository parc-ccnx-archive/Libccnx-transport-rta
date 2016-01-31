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
 */
#include <config.h>

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>

#include <LongBow/runtime.h>

#include <ccnx/api/control/cpi_NameRouteType.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Base64.h>

#include "controlPlaneInterface.h"
#include "cpi_private.h"

struct name_route_type_s {
    const char *str;
    CPINameRouteType type;
} nameRouteTypeString[] = {
    { .str = "EXACT",   .type = cpiNameRouteType_EXACT_MATCH   },
    { .str = "LONGEST", .type = cpiNameRouteType_LONGEST_MATCH },
    { .str = "DEFAULT", .type = cpiNameRouteType_DEFAULT       },
    { .str = NULL,      .type = 0                              }
};

const char *
cpiNameRouteType_ToString(CPINameRouteType type)
{
    for (int i = 0; nameRouteTypeString[i].str != NULL; i++) {
        if (nameRouteTypeString[i].type == type) {
            return nameRouteTypeString[i].str;
        }
    }
    trapIllegalValue(type, "Unknown type: %d", type);
}

CPINameRouteType
cpiNameRouteType_FromString(const char *str)
{
    for (int i = 0; nameRouteTypeString[i].str != NULL; i++) {
        if (strcasecmp(nameRouteTypeString[i].str, str) == 0) {
            return nameRouteTypeString[i].type;
        }
    }
    trapIllegalValue(type, "Unknown type: %s", str);
}
