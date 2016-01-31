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
 * @file rta_Framework_private.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_Framework_private_h
#define Libccnx_rta_Framework_private_h

#include <stdlib.h>
#include <sys/queue.h>
#include <pthread.h>

#include "rta_ProtocolStack.h"
#include "rta_Connection.h"
#include "rta_Framework_Services.h"

#include "rta_ConnectionTable.h"

#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_Event.h>
#include <parc/algol/parc_EventTimer.h>
#include <parc/algol/parc_EventSignal.h>

// the router's wrapped time frquency is 1 msec
#define WTHZ 1000
#define FC_MSEC_PER_TICK (1000 / WTHZ)
#define FC_USEC_PER_TICK (1000000 / WTHZ)
#define MSEC_TO_TICKS(msec) ((msec < FC_MSEC_PER_TICK) ? 1 : msec / FC_MSEC_PER_TICK)

// ===================================================

typedef struct framework_protocol_holder {
    RtaProtocolStack   *stack;
    uint64_t kv_hash;
    int stack_id;

    TAILQ_ENTRY(framework_protocol_holder)    list;
} FrameworkProtocolHolder;


struct rta_framework {
    PARCRingBuffer1x1           *commandRingBuffer;
    PARCNotifier                *commandNotifier;
    PARCEvent                   *commandEvent;

    //struct event_config         *cfg;
    int udp_socket;

    PARCEventScheduler          *base;

    PARCEventSignal         *signal_int;
    PARCEventSignal         *signal_usr1;
    PARCEventTimer          *tick_event;
    PARCEvent               *udp_event;
    PARCEventTimer          *transmit_statistics_event;
    PARCEventSignal         *signal_pipe;

    struct timeval starttime;
    ticks clock_ticks;                       // at WTHZ

    // used by seed48 and nrand48
    unsigned short seed[3];

    pthread_t thread;

    unsigned connid_next;

    // operations that modify global state need
    // to be locked.
    pthread_mutex_t status_mutex;
    pthread_cond_t status_cv;
    RtaFrameworkStatus status;

    // signals from outside control thread to event scheduler
    // that it should exit its event loop.  This does
    // not need to be protected in mutex (its not
    // a condition variable).  We check for this
    // inside the HZ timer callback.
    bool killme;

    // A list of all our in-use protocol stacks
    TAILQ_HEAD(, framework_protocol_holder)    protocols_head;

    RtaConnectionTable *connectionTable;

    RtaLogger *logger;
};

int rtaFramework_CloseConnection(RtaFramework *framework, RtaConnection *connection);

/**
 * Lock the frameworks state machine status
 *
 * Will block until the state machine status is locked
 *
 * @param [in] framework An allocated framework
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rta_Framework_LockStatus(RtaFramework *framework);

/**
 * Unlock the state mahcines status
 *
 * Will assert if we do not currently hold the lock
 *
 * @param [in] framework An allocated framework
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rta_Framework_UnlockStatus(RtaFramework *framework);

/**
 * Wait on the state machine's condition variable to be signaled
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] framework An allocated framework
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rta_Framework_WaitStatus(RtaFramework *framework);

/**
 * Broadcast on the state machine's condition variable (signal it)
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] framework An allocated framework
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rta_Framework_BroadcastStatus(RtaFramework *framework);
#endif
