/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file rta_ComponentStats.h
 * @brief <#Brief Description#>
 *
 * Statistics are PER CONNECTION PER COMPONENT.  Therefore, a component would call
 * rtaConnection_GetStats(conn, component) to access its stats.  Each component must
 * create its stats counter in _Open and free it in _Close.
 *
 * Each ProtocolStack has a PER STACK PER COMPONENT set of statistics too.  When a
 * component creates its stats in _Open, it passes a pointer to its stack, so when
 * _Increment is called, it will increment both the component's stats and the stack's
 * stats.
 *
 * For example:
 *
 *    protocolStack_Init() creates stack-wide stats for each component type.
 *    componentX_Open(stack) creates per-connection stats for that component with
 *      a reference to stack using stats_Create(stack, component_type)
 *    componentX_Y(conn) performs some per-connection activity.  It would call
 *      stats_Increment(rtaConnection_GetStats(conn), component_type, stat_type).
 *      That would increment the per-connection per-component stat and if the stack
 *      was not null, would increment the identical component_type, stat_type
 *      stat in the per-stack per-component counters.
 *
 *
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_ComponentStats
#define Libccnx_rta_ComponentStats

#include <ccnx/transport/transport_rta/core/components.h>

struct protocol_stack;

struct rta_component_stats;
/**
 *
 * @see stats_Create
 */
typedef struct rta_component_stats RtaComponentStats;

typedef enum {
    STATS_OPENS,
    STATS_CLOSES,
    STATS_UPCALL_IN,
    STATS_UPCALL_OUT,
    STATS_DOWNCALL_IN,
    STATS_DOWNCALL_OUT,
    STATS_LAST              // must be last
} RtaComponentStatType;

/**
 * Create a stats component
 *
 * If the optional stack is specified, its statistics will be incremented whenever this
 * stats object is incremented.  Otherwise, it may be NULL.
 *
 * @param [in] stack Optional protocol stack
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
RtaComponentStats *rtaComponentStats_Create(struct protocol_stack *stack, RtaComponents componentType);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
char *rtaComponentStatType_ToString(RtaComponentStatType statType);

/**
 * Increment and return incremented value
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
uint64_t rtaComponentStats_Increment(RtaComponentStats *stats, RtaComponentStatType statType);

/**
 * Return value
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
uint64_t rtaComponentStats_Get(RtaComponentStats *stats, RtaComponentStatType statType);

/**
 * dump the stats to the given output
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
void rtaComponentStats_Dump(RtaComponentStats *stats, FILE *output);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 * @see <#references#>
 */
void rtaComponentStats_Destroy(RtaComponentStats **statsPtr);
#endif
