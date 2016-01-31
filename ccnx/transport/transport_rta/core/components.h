/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
//
//  components.h
//  Libccnx
//
//  Created by Mosko, Marc <Marc.Mosko@parc.com> on 9/19/13.


#ifndef Libccnx_components_h
#define Libccnx_components_h

// Every component in the system must be defined here
// These must correspond to array indicies.
typedef enum {
    API_CONNECTOR = 0,
    FC_NONE = 1,
    FC_VEGAS = 2,
    FC_PIPELINE = 3,
    // vacant          = 4,
    // vacant          = 5,
    // vacant          = 6,
    CODEC_NONE = 7,
    CODEC_UNSPEC = 8,
    CODEC_TLV = 9,
    // vacant          = 10,
    // vacant          = 11,
    FWD_NONE = 12,
    FWD_LOCAL = 13,
    // vacant          = 14,
    // vacant          = 15,
    TESTING_UPPER = 16,
    TESTING_LOWER = 17,
    FWD_METIS = 19,
    LAST_COMPONENT = 20,      // MUST ALWAYS BE LAST
    UNKNOWN_COMPONENT         // MUST BE VERY LAST
} RtaComponents;


// This is defied in rta_ProtocolStack.c and should be kept
// in sync with RtaComponents
extern const char *RtaComponentNames[LAST_COMPONENT];

#endif
