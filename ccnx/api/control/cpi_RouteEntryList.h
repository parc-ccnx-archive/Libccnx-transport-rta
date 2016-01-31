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
 * @file cpi_RouteEntryList.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_RouteEntryList_h
#define libccnx_cpi_RouteEntryList_h

struct cpi_route_entry_list;
typedef struct cpi_route_entry_list CPIRouteEntryList;

#include <ccnx/api/control/cpi_RouteEntry.h>

CPIRouteEntryList *cpiRouteEntryList_Create();
void cpiRouteEntryList_Destroy(CPIRouteEntryList **listPtr);

/**
 * Adds a route entry to the list.
 *
 *   Appends <code>entry</code> to the list.  Takes ownership of the entry
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void cpiRouteEntryList_Append(CPIRouteEntryList *list, CPIRouteEntry *entry);

/**
 * Determine if two CPIRouteEntryList instances are equal.
 *
 * Two CPIRouteEntryList instances are equal if, and only if,
 * * ...
 *
 * The following equivalence relations on non-null `CPIRouteEntryList` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIRouteEntryList_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `cpiRouteEntryList_Equals(x, y)` must return true if and only if
 *        `cpiRouteEntryList_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiRouteEntryList_Equals(x, y)` returns true and
 *        `cpiRouteEntryList_Equals(y, z)` returns true,
 *        then  `cpiRouteEntryList_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiRouteEntryList_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiRouteEntryList_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIRouteEntryList` instance.
 * @param b A pointer to a `CPIRouteEntryList` instance.
 * @return true if the two `CPIRouteEntryList` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIRouteEntryList *a = cpiRouteEntryList_Create();
 *    CPIRouteEntryList *b = cpiRouteEntryList_Create();
 *
 *    if (cpiRouteEntryList_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool cpiRouteEntryList_Equals(const CPIRouteEntryList *a, const CPIRouteEntryList *b);

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
size_t cpiRouteEntryList_Length(const CPIRouteEntryList *list);

/**
 * Returns a reference counted copy of the route entry.
 *
 *   Caller must destroy the returned value.
 *   Will assert if you go beyond the end of the list.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIRouteEntry *cpiRouteEntryList_Get(CPIRouteEntryList *list, size_t index);


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
PARCJSON *cpiRouteEntryList_ToJson(const CPIRouteEntryList *list);

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
CPIRouteEntryList *cpiRouteEntryList_FromJson(PARCJSON *json);
#endif // libccnx_cpi_RouteEntryList_h
