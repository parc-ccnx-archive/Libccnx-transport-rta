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
 * This test rig sets up a mock RTA Framework for testing Components and Connectors.
 *
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <ccnx/transport/transport_rta/core/rta_Framework.h>
#include <ccnx/transport/transport_rta/core/rta_Framework_Commands.c>
#include <ccnx/transport/transport_rta/core/rta_Framework_NonThreaded.h>

#ifndef MAXPATH
#define MAXPATH 1024
#endif

typedef struct mock_framework {
    PARCRingBuffer1x1*commandRingBuffer;
    PARCNotifier     *commandNotifier;
    RtaFramework     *framework;

    int stackId;
    RtaProtocolStack *stack;

    int connection_fds[2];
    RtaConnection    *connection;

    CCNxTransportConfig  *transport_config;
} MockFramework;

static MockFramework *
mockFramework_Create(CCNxTransportConfig *config)
{
    MockFramework *mock = parcMemory_AllocateAndClear(sizeof(MockFramework));
    assertNotNull(mock, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(MockFramework));

    mock->transport_config = ccnxTransportConfig_Copy(config);
    assertNotNull(mock->transport_config, "%s got null params from createParams\n", __func__);

    mock->commandRingBuffer = parcRingBuffer1x1_Create(128, NULL);
    mock->commandNotifier = parcNotifier_Create();
    mock->framework = rtaFramework_Create(mock->commandRingBuffer, mock->commandNotifier);

    // Create the protocol stack

    mock->stackId = 1;
    RtaCommandCreateProtocolStack *createStack =
        rtaCommandCreateProtocolStack_Create(mock->stackId, ccnxTransportConfig_GetStackConfig(mock->transport_config));
    _rtaFramework_ExecuteCreateStack(mock->framework, createStack);
    rtaCommandCreateProtocolStack_Release(&createStack);

    // peek inside and get the protocol stack reference
    FrameworkProtocolHolder *fph = rtaFramework_GetProtocolStackByStackId(mock->framework, mock->stackId);
    mock->stack = fph->stack;

    int error = socketpair(AF_UNIX, SOCK_STREAM, 0, mock->connection_fds);
    assertFalse(error, "Error creating socket pair: (%d) %s", errno, strerror(errno));

    RtaCommandOpenConnection *openConnection = rtaCommandOpenConnection_Create(mock->stackId, mock->connection_fds[0], mock->connection_fds[1],
                                                                               ccnxConnectionConfig_GetJson(ccnxTransportConfig_GetConnectionConfig(mock->transport_config)));
    _rtaFramework_ExecuteOpenConnection(mock->framework, openConnection);
    rtaCommandOpenConnection_Release(&openConnection);

    mock->connection = rtaConnectionTable_GetByApiFd(mock->framework->connectionTable, mock->connection_fds[0]);

    // Uses the non-threaded forwarder, make sure we step at least once
    rtaFramework_NonThreadedStep(mock->framework);

    return mock;
}

static void
mockFramework_Destroy(MockFramework **mockPtr)
{
    MockFramework *mock = *mockPtr;

    rtaFramework_Teardown(mock->framework);

    parcRingBuffer1x1_Release(&mock->commandRingBuffer);
    parcNotifier_Release(&mock->commandNotifier);

    rtaFramework_Destroy(&mock->framework);
    ccnxTransportConfig_Destroy(&mock->transport_config);

    parcMemory_Deallocate((void **) &mock);
}
