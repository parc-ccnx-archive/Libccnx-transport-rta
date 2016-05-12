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
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>

#include <LongBow/runtime.h>
#include <parc/algol/parc_Memory.h>

#include <ccnx/transport/common/transport.h>
#include <ccnx/transport/common/transport_private.h>
#include <ccnx/transport/common/transport_Message.h>

#define DEBUG_OUTPUT 0

struct transport_message {
    CCNxTlvDictionary *dictionary;
    TransportMessage_Free *freefunc;
    void *info;

    struct timeval creationTime;
};

static size_t _transport_messages_created = 0;
static size_t _transport_messages_destroyed = 0;

static void
_transportMessage_GetTimeOfDay(struct timeval *outputTime)
{
#ifdef DEBUG
    // if in debug mode, time messages
    gettimeofday(outputTime, NULL);
#else
    *outputTime = (struct timeval) { 0, 0 };
#endif
}

TransportMessage *
transportMessage_CreateFromDictionary(CCNxTlvDictionary *dictionary)
{
    assertNotNull(dictionary, "Cannot create TransportMessage from NULL Dictionary");
    if (dictionary == NULL) {
        return NULL;
    }

    TransportMessage *tm = parcMemory_AllocateAndClear(sizeof(TransportMessage));

    if (tm != NULL) {
        tm->dictionary = ccnxTlvDictionary_Acquire(dictionary);

        _transportMessage_GetTimeOfDay(&tm->creationTime);

        _transport_messages_created++;

        if (DEBUG_OUTPUT) {
            printf("%-35s allocs %zu destroys %zu pointer %p dict   %p\n",
                   __func__,
                   _transport_messages_created,
                   _transport_messages_destroyed,
                   (void *) tm,
                   (void *) dictionary);
        }
    }

    return tm;
}

CCNxTlvDictionary *
transportMessage_GetDictionary(TransportMessage *tm)
{
    assertNotNull(tm, "TransportMessage_GetWireMessage called on NULL transport message");
    return tm->dictionary;
}

bool
transportMessage_isValid(const TransportMessage *message)
{
    bool result = false;
    if (message != NULL) {
        result = true;
    }

    return result;
}

void
transportMessage_AssertValid(const TransportMessage *message)
{
    assertTrue(transportMessage_isValid(message), "TransportMessage @ %p is invalid.", (void *) message);
}

/*
 * Frees the TransportMessage wrapper, but user is responsible
 * for destroying the inner pieces.
 */
static void
_transportMessage_Destroy(TransportMessage **msgPtr)
{
    assertNotNull(msgPtr, "TransportMessage_Destroy called on NULL transport message pointer");
    if (msgPtr != NULL) {
        TransportMessage *msg = *msgPtr;
        transportMessage_OptionalAssertValid(msg);

        _transport_messages_destroyed++;

        if (DEBUG_OUTPUT) {
            printf("%-35s allocs %zu destroys %zu pointer %p\n",
                   __func__,
                   _transport_messages_created,
                   _transport_messages_destroyed,
                   (void *) msg);
        }

        if (msg->freefunc != NULL) {
            msg->freefunc(&msg->info);
        }

        parcMemory_Deallocate((void **) &msg);
        *msgPtr = NULL;
    }
}

void
transportMessage_Destroy(TransportMessage **tmPtr)
{
    TransportMessage *tm = *tmPtr;
    assertNotNull(tmPtr, "called with NULL transport message double pointer");

    assertNotNull(tm, "called with NULL transport message dereference");

    if (tm->dictionary != NULL) {
        ccnxTlvDictionary_Release(&tm->dictionary);
        tm->dictionary = NULL;
    }

    _transportMessage_Destroy(tmPtr);
}

/*
 * Add some stack payload to a transport message.  Will not be freed.
 */
void
transportMessage_SetInfo(TransportMessage *tm, void *info, TransportMessage_Free *freefunc)
{
    assertNotNull(tm, "%s called with NULL transport message", __func__);
    tm->info = info;
    tm->freefunc = freefunc;
}

void *
transportMessage_GetInfo(const TransportMessage *tm)
{
    assertNotNull(tm, "%s called with NULL transport message", __func__);
    return tm->info;
}


struct timeval
transportMessage_GetDelay(const TransportMessage *tm)
{
    struct timeval now;
    _transportMessage_GetTimeOfDay(&now);
    timersub(&now, &tm->creationTime, &now);
    return now;
}

bool
transportMessage_IsControl(const TransportMessage *tm)
{
    return ccnxTlvDictionary_IsControl(tm->dictionary);
}

bool
transportMessage_IsInterest(const TransportMessage *tm)
{
    return ccnxTlvDictionary_IsInterest(tm->dictionary);
}

bool
transportMessage_IsContentObject(const TransportMessage *tm)
{
    return ccnxTlvDictionary_IsContentObject(tm->dictionary);
}
