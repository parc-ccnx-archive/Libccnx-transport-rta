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
 * Implements the RtaCommandTransmitStatistics object which signals to RTA Framework to open a new connection
 * with the given configuration.
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <sys/param.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <ccnx/transport/transport_rta/commands/rta_CommandTransmitStatistics.h>

struct rta_command_transmitstatistics {
    struct timeval period;
    char *filename;
};

// ======= Private API

static void
_rtaCommandTransmitStatistics_Destroy(RtaCommandTransmitStatistics **transmitStatsPtr)
{
    RtaCommandTransmitStatistics *transmitStats = *transmitStatsPtr;
    parcMemory_Deallocate((void **) &(transmitStats->filename));
}

parcObject_ExtendPARCObject(RtaCommandTransmitStatistics, _rtaCommandTransmitStatistics_Destroy,
                            NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(rtaCommandTransmitStatistics, RtaCommandTransmitStatistics);

parcObject_ImplementRelease(rtaCommandTransmitStatistics, RtaCommandTransmitStatistics);

// ======= Public API

RtaCommandTransmitStatistics *
rtaCommandTransmitStatistics_Create(struct timeval period, const char *filename)
{
    assertNotNull(filename, "Filename must be non-null");

    RtaCommandTransmitStatistics *transmitStats = parcObject_CreateInstance(RtaCommandTransmitStatistics);
    memcpy(&transmitStats->period, &period, sizeof(struct timeval));
    transmitStats->filename = parcMemory_StringDuplicate(filename, PATH_MAX);

    return transmitStats;
}

struct timeval
rtaCommandTransmitStatistics_GetPeriod(const RtaCommandTransmitStatistics *transmitStats)
{
    assertNotNull(transmitStats, "Parameter transmitStats must be non-null");
    return transmitStats->period;
}

const char *
rtaCommandTransmitStatistics_GetFilename(const RtaCommandTransmitStatistics *transmitStats)
{
    assertNotNull(transmitStats, "Parameter transmitStats must be non-null");
    return transmitStats->filename;
}
