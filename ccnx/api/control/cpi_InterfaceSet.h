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
 * @file cpi_InterfaceSet.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_InterfaceSet_h
#define libccnx_cpi_InterfaceSet_h

#include <ccnx/api/control/cpi_Interface.h>

struct cpi_interface_set;
/**
 *
 * @see cpiInterfaceSet_Create
 */
typedef struct cpi_interface_set CPIInterfaceSet;

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
CPIInterfaceSet *cpiInterfaceSet_Create(void);

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
void cpiInterfaceSet_Destroy(CPIInterfaceSet **setPtr);

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
CPIInterfaceSet *cpiInterfaceSet_FromJson(PARCJSON *json);

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
PARCJSON *cpiInterfaceSet_ToJson(CPIInterfaceSet *iface);

/**
 * Adds interface to set, does not allow duplicates
 *
 *   Takes ownership of the iface memory if added
 *
 *   Duplicates are two entries with the same interface index
 *
 * @param <#param1#>
 * @return true if added, false if not (likely a duplicate)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool cpiInterfaceSet_Add(CPIInterfaceSet *set, CPIInterface *iface);

/**
 * The number of interfaces in the set
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
size_t cpiInterfaceSet_Length(const CPIInterfaceSet *set);

/**
 * Uses the ordinal index of the interface in the Set
 *
 *   Ranges from 0 .. <code>cpiInterfaceSet_Length()-1</code>.
 *
 * @param <#param1#>
 * @return NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *cpiInterfaceSet_GetByOrdinalIndex(CPIInterfaceSet *set, size_t ordinalIndex);

/**
 * Retreives by the CPI assigned interface index
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *cpiInterfaceSet_GetByInterfaceIndex(const CPIInterfaceSet *set, unsigned interfaceIndex);

/**
 * Uses the system name (e.g. "en0")
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *cpiInterfaceSet_GetByName(CPIInterfaceSet *set, const char *name);


/**
 * Determine if two CPIInterfaceSet instances are equal.
 *
 * Two CPIInterfaceSet instances are equal if, and only if, the sets contain the same elements
 * - order independent.
 * Each element is compared via <code>cpiInterface_Equals()</code>
 *
 * The following equivalence relations on non-null `CPIInterfaceSet` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIInterfaceSet_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `CPIInterfaceSet_Equals(x, y)` must return true if and only if
 *        `cpiInterfaceSet_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiInterfaceSet_Equals(x, y)` returns true and
 *        `cpiInterfaceSet_Equals(y, z)` returns true,
 *        then  `cpiInterfaceSet_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiInterfaceSet_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiInterfaceSet_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIInterfaceSet` instance.
 * @param b A pointer to a `CPIInterfaceSet` instance.
 * @return true if the two `CPIInterfaceSet` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIInterfaceSet *a = cpiInterfaceSet_Create();
 *    CPIInterfaceSet *b = cpiInterfaceSet_Create();
 *
 *    if (cpiInterfaceSet_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool cpiInterfaceSet_Equals(const CPIInterfaceSet *a, const CPIInterfaceSet *b);
#endif // libccnx_cpi_InterfaceSet_h
