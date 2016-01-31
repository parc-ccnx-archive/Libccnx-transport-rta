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
 * @file cpi_InterfaceTypes.h
 * @brief CPI INterface Types
 *
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libccnx_cpi_InterfaceTypes_h
#define libccnx_cpi_InterfaceTypes_h

typedef enum {
    CPI_IFACE_LOOPBACK = 1,
    CPI_IFACE_ETHERNET = 2,
    CPI_IFACE_LOCALAPP = 3,
    CPI_IFACE_TUNNEL = 4,
    CPI_IFACE_GROUP = 5
} CPIInterfaceType;

typedef enum {
    CPI_IFACE_UNKNOWN = 0,
    CPI_IFACE_UP = 1,
    CPI_IFACE_DOWN = 2
} CPIInterfaceStateType;

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param [in] type An instance of `CPIInterfaceType`.
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiInterfaceType_ToString(CPIInterfaceType type);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param str A nul-terminated C string.
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterfaceType cpiInterfaceType_FromString(const char *str);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param type A CPIInterfaceStateType value.
 * @return A nul-terminated C string.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char *cpiInterfaceStateType_ToString(CPIInterfaceStateType type);

/**
 * <#OneLineDescription#>
 *
 *   <#Discussion#>
 *
 * @param str A nul-terminated C string.
 * @return <#return#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
CPIInterfaceStateType cpiInterfaceStateType_FromString(const char *str);
#endif // libccnx_cpi_InterfaceTypes_h
