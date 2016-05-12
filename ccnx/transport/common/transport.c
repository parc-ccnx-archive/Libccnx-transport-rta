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

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>

#include "transport.h"
#include "transport_private.h"
#include "rta_Transport.h"

struct transport_context {
    TransportTypes transport_type;
    struct transport_operations ops;
    void *transport_data;
    unsigned references;
};

// the one global transport, for now
static TransportContext *the_context = NULL;

TransportContext *
Transport_Create(TransportTypes type)
{
    if (the_context == NULL) {
        switch (type) {
            case TRANSPORT_RTA:
                the_context = parcMemory_Allocate(sizeof(TransportContext));
                assertNotNull(the_context, "TransportContext could not be allocated, parcMemory_Allocate(%zu) returned NULL", sizeof(TransportContext));

                the_context->references = 0;
                the_context->ops = rta_ops;
                the_context->transport_data = the_context->ops.Create();
                the_context->transport_type = type;
                break;

            default:
                fprintf(stderr, "%s unknown transport type %d\n", __func__, type);
                abort();
                break;
        }
    }

    if (the_context->transport_type == type) {
        the_context->references++;
        return the_context;
    }

    fprintf(stderr, "%s transport type %d not of request type %d\n",
            __func__, the_context->transport_type, type);
    abort();
}

int
Transport_Open(CCNxTransportConfig *transportConfig)
{
    assertNotNull(the_context, "The TransportContext is NULL.");
    assertNotNull(transportConfig, "The parameter transportConfig must be a non-null CCNxTransportConfig pointer");
    return the_context->ops.Open(the_context->transport_data, transportConfig);
}

int
Transport_Send(int desc, CCNxMetaMessage *msg_in)
{
    assertNotNull(the_context, "the_context is null");
    return the_context->ops.Send(the_context->transport_data, desc, msg_in, CCNxStackTimeout_Never);
}

TransportIOStatus
Transport_Recv(int desc, CCNxMetaMessage **msg_out)
{
    return the_context->ops.Recv(the_context->transport_data, desc, msg_out, CCNxStackTimeout_Never);
}

int
Transport_Close(int desc)
{
    return the_context->ops.Close(the_context->transport_data, desc);
}

int
Transport_PassCommand(void *stackCommand)
{
    return the_context->ops.PassCommand(the_context->transport_data, stackCommand);
}

void
Transport_Destroy(TransportContext **ctxPtr)
{
    assertNotNull(ctxPtr, "Transport_Destroy called with null context");
    assertNotNull(*ctxPtr, "Transport_Destroy callled with reference to null");

    TransportContext *ctx = *ctxPtr;

    assertTrue(the_context == ctx, "Passed ctx is not the same");
    assertTrue(ctx->references > 0, "Invalid reference count");

    ctx->references--;
    if (ctx->references == 0) {
        ctx->ops.Destroy(&ctx->transport_data);
        memset(ctx, 0, sizeof(TransportContext));
        parcMemory_Deallocate((void **) &ctx);
        ctx = NULL;
        the_context = NULL;
    }
}
