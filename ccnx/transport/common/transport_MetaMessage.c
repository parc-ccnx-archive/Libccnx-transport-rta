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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <ccnx/transport/common/transport_MetaMessage.h>
#include <ccnx/common/codec/ccnxCodec_TlvPacket.h>
#include <ccnx/common/ccnx_WireFormatMessage.h>
#include <ccnx/common/ccnx_Manifest.h>

CCNxMetaMessage *
ccnxMetaMessage_CreateFromInterest(const CCNxInterest *interest)
{
    return ccnxMetaMessage_Acquire((CCNxMetaMessage *) interest);
}

CCNxMetaMessage *
ccnxMetaMessage_CreateFromContentObject(const CCNxContentObject *contentObject)
{
    return ccnxMetaMessage_Acquire((CCNxMetaMessage *) contentObject);
}

CCNxMetaMessage *
ccnxMetaMessage_CreateFromControl(const CCNxControl *control)
{
    return ccnxMetaMessage_Acquire((CCNxMetaMessage *) control);
}

CCNxMetaMessage *
ccnxMetaMessage_CreateFromManifest(const CCNxManifest *manifest)
{
    return ccnxMetaMessage_Acquire((CCNxMetaMessage *) manifest);
}


CCNxContentObject *
ccnxMetaMessage_GetContentObject(const CCNxMetaMessage *message)
{
    return (CCNxContentObject *) message;
}

CCNxInterest *
ccnxMetaMessage_GetInterest(const CCNxMetaMessage *message)
{
    return (CCNxInterest *) message;
}

CCNxInterestReturn *
ccnxMetaMessage_GetInterestReturn(const CCNxMetaMessage *message)
{
    return (CCNxInterestReturn *) message;
}

CCNxControl *
ccnxMetaMessage_GetControl(const CCNxMetaMessage *message)
{
    return (CCNxControl *) message;
}

CCNxManifest *
ccnxMetaMessage_GetManifest(const CCNxMetaMessage *message)
{
    return (CCNxManifest *) message;
}

CCNxMetaMessage *
ccnxMetaMessage_Acquire(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_Acquire(message);
}

void
ccnxMetaMessage_Release(CCNxMetaMessage **messagePtr)
{
    ccnxTlvDictionary_Release(messagePtr);
}

void
ccnxMetaMessage_Display(const CCNxMetaMessage *message, int indentation)
{
    ccnxTlvDictionary_Display(message, indentation);
}

bool
ccnxMetaMessage_IsContentObject(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_IsContentObject(message);
}

bool
ccnxMetaMessage_IsInterest(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_IsInterest(message);
}

bool
ccnxMetaMessage_IsInterestReturn(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_IsInterestReturn(message);
}

bool
ccnxMetaMessage_IsControl(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_IsControl(message);
}

bool
ccnxMetaMessage_IsManifest(const CCNxMetaMessage *message)
{
    return ccnxTlvDictionary_IsManifest(message);
}

/**
 * Given an iovec encoded version of a TlvDictionary, which is what we get when we call ccnxCodecTlvPacket_DictionaryEncode(),
 * linearize it into a PARCBuffer so we can treat it as a PARCBuffer.
 */
static PARCBuffer *
_iovecToParcBuffer(const CCNxCodecNetworkBufferIoVec *iovec)
{
    PARCBuffer *result = NULL;

    size_t iovcnt = ccnxCodecNetworkBufferIoVec_GetCount((CCNxCodecNetworkBufferIoVec *) iovec);
    const struct iovec *array = ccnxCodecNetworkBufferIoVec_GetArray((CCNxCodecNetworkBufferIoVec *) iovec);

    size_t totalbytes = 0;
    for (int i = 0; i < iovcnt; i++) {
        totalbytes += array[i].iov_len;
    }

    result = parcBuffer_Allocate(totalbytes);
    for (int i = 0; i < iovcnt; i++) {
        parcBuffer_PutArray(result, array[i].iov_len, array[i].iov_base);
    }

    parcBuffer_Flip(result);


    return result;
}

CCNxMetaMessage *
ccnxMetaMessage_CreateFromWireFormatBuffer(PARCBuffer *rawMessage)
{
    CCNxMetaMessage *result = NULL;

    CCNxWireFormatMessage *message = ccnxWireFormatMessage_Create(rawMessage);

    if (message != NULL) {
        // Get the dictionary from the ccnxWireFormatMessage.
        CCNxTlvDictionary *dictionary = ccnxWireFormatMessage_GetDictionary(message);

        // We have a partially unpacked dictionary now, but we need to more fully unpack it for our processing.
        bool success = ccnxCodecTlvPacket_BufferDecode(rawMessage, dictionary);

        if (success) {
            result = (CCNxMetaMessage *) dictionary;
        } else {
            ccnxWireFormatMessage_Release(&message);
            result = NULL;
        }
    }

    return result;
}

PARCBuffer *
ccnxMetaMessage_CreateWireFormatBuffer(CCNxMetaMessage *message, PARCSigner *signer)
{
    CCNxCodecNetworkBufferIoVec *iovec = ccnxCodecTlvPacket_DictionaryEncode(message, signer);

    // iovec has the wireformat version of 'interest' now.

    PARCBuffer *result = _iovecToParcBuffer(iovec);

    ccnxCodecNetworkBufferIoVec_Release(&iovec);

    return result;
}
