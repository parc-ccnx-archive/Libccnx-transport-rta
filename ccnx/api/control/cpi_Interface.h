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
 * @file cpi_Interface.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_Interface_h
#define libccnx_cpi_Interface_h

#include <ccnx/api/control/cpi_Address.h>
#include <ccnx/api/control/cpi_AddressList.h>

struct cpi_interface;
typedef struct cpi_interface CPIInterface;

/**
 * Creates a representation of an interface
 *
 *   The name is copied.  Creates a representation of a system interface.
 *
 * @param <#param1#>
 * @return An allocated object, you must call <code>cpiInterface_Destroy()</code>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *cpiInterface_Create(const char *name, unsigned interfaceIndex, bool loopback, bool supportMulticast, unsigned mtu);

void cpiInterface_Destroy(CPIInterface **interfacePtr);

/**
 * Creates an Interface object based on a JSON description.
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return An allocated object, you must call <code>cpiInterface_Destroy()</code>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterface *cpiInterface_FromJson(PARCJSON *json);

/**
 * Creates a JSON description of the object
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return An allocated object, you must destroy it.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCJSON *cpiInterface_ToJson(CPIInterface *iface);

/**
 * Adds an address to an interface
 *
 *   Does not allow duplicates, if already exists is not added again
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void cpiInterface_AddAddress(CPIInterface *iface, CPIAddress *address);

/**
 * Retrieves a list of interface addresses
 *
 *   <#Discussion#>
 *
 * @param <#param1#>
 * @return Will not be NULL, but may be empty
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const CPIAddressList *cpiInterface_GetAddresses(const CPIInterface *iface);

/**
 * The interface index
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
unsigned cpiInterface_GetInterfaceIndex(const CPIInterface *iface);

/**
 * Returns the interface name, e.g. "eth0"
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] iface An allocated CPIInterface
 *
 * @return non-null The interface Name as a C-string
 * @return null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiInterface_GetName(const CPIInterface *iface);

/**
 * Returns the Maximum Transmission Unit (MTU) of the interface
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] iface An allocated CPIInterface
 *
 * @return number The MTU as reported by the kernel
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
unsigned cpiInterface_GetMTU(const CPIInterface *iface);

/**
 * Determine if two CPIInterfaceName instances are equal.
 *
 *
 * The following equivalence relations on non-null `CPIInterfaceName` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `CPIInterfaceName_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `cpiInterfaceName_Equals(x, y)` must return true if and only if
 *        `cpiInterfaceName_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `cpiInterfaceName_Equals(x, y)` returns true and
 *        `cpiInterfaceName_Equals(y, z)` returns true,
 *        then  `cpiInterfaceName_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `cpiInterfaceName_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `cpiInterfaceName_Equals(x, NULL)` must
 *      return false.
 *
 * @param a A pointer to a `CPIInterfaceName` instance.
 * @param b A pointer to a `CPIInterfaceName` instance.
 * @return true if the two `CPIInterfaceName` instances are equal.
 *
 * Example:
 * @code
 * {
 *    CPIInterfaceName *a = cpiInterfaceName_Create();
 *    CPIInterfaceName *b = cpiInterfaceName_Create();
 *
 *    if (cpiInterfaceName_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool cpiInterface_NameEquals(const CPIInterface *iface, const char *name);

/**
 * Two CPIInterfaces are idential
 *
 *   All properties must be the same.  The order of addresses matters, and
 *   they must have been added to the address list in the same order.
 *
 *   The interface name match is case in-sensitive.
 *
 * @param <#param1#>
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool cpiInterface_Equals(const CPIInterface *a, const CPIInterface *b);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param interface A CPIInterface structure pointer.
 * @return An allocate string representation of the CPIInterface that must be freed via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
char *cpiInterface_ToString(const CPIInterface *interface);
#endif // libccnx_cpi_Interface_h
