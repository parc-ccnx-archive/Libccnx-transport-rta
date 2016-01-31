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
 * @file ccnx_ConnectionConfig.
 * @brief Tranport Stack Connection configuration information.
 *
 * These are subsystems instantiated within components
 * They define per-connection behavior, not stack structure.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef TransportRTA_connectionConfig_h
#define TransportRTA_connectionConfig_h


struct ccnx_connection_config;
typedef struct ccnx_connection_config CCNxConnectionConfig;

/**
 * Create a `CCNxConnectionConfig` instance.
 *
 * The instance must be populated with configuration information before it can be used.
 *
 *
 * @return NULL An error occurred.
 * @return non-NULL A valid `CCNxConnectionConfig` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxConnectionConfig *config = ccnxConnectionConfig_Create();
 *
 *     ccnxConnectionConfig_Destroy(&config);
 * @endcode
 */
CCNxConnectionConfig *ccnxConnectionConfig_Create(void);

/**
 * Destroy previously created `CCNxConnectionConfig` instance.
 *
 * @param [in] configPtr A pointer to a pointer to a valid `CCNxConnectionConfig` instance that will be set to zero upon return.
 *
 * Example:
 * @code
 * {
 *     CCNxConnectionConfig *config = ccnxConnectionConfig_Create();
 *
 *     ccnxConnectionConfig_Destroy(&config);
 * @endcode
 */
void ccnxConnectionConfig_Destroy(CCNxConnectionConfig **configPtr);

#ifdef CCNxTransport_DISABLE_VALIDATION
#  define ccnxConnectionConfig_OptionalAssertValid(_instance_)
#else
#  define ccnxConnectionConfig_OptionalAssertValid(_instance_) ccnxConnectionConfig_AssertValid(_instance_)
#endif

/**
 * Determine if an instance of `CCNxTransportConfig` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] config A pointer to a `CCNxTransportConfig` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxConnectionConfig *config = ccnxConnectionConfig_Create();
 *     ccnxConnectionConfig_IsValid(config);
 *     ccnxConnectionConfig_Destroy(&config);
 * }
 * @endcode
 */
bool ccnxConnectionConfig_IsValid(const CCNxConnectionConfig *config);

/**
 * Assert that an instance of `CCNxTransportConfig` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] config A pointer to a `CCNxTransportConfig` instance.
 *
 * Example:
 * @code
 * {
 *     CCNxConnectionConfig *config = ccnxConnectionConfig_Create();
 *     ccnxConnectionConfig_AssertValid(config);
 *     ccnxConnectionConfig_Destroy(&config);
 * }
 * @endcode
 */
void ccnxConnectionConfig_AssertValid(const CCNxConnectionConfig *config);

/**
 * Determine if two `CCNxConnectionConfig` instances are equal.
 *
 * The following equivalence relations on non-null `CCNxConnectionConfig` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `ccnxConnectionConfig_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `ccnxConnectionConfig_Equals(x, y)` must return true if and only if
 *        `ccnxConnectionConfig_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `ccnxConnectionConfig_Equals(x, y)` returns true and
 *        `ccnxConnectionConfig_Equals(y, z)` returns true,
 *        then `ccnxConnectionConfig_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `ccnxConnectionConfig_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `ccnxConnectionConfig_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid CCNxConnectionConfig instance.
 * @param [in] y A pointer to a valid CCNxConnectionConfig instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     CCNxConnectionConfig *a = ccnxConnectionConfig_Create();
 *     CCNxConnectionConfig *b = ccnxConnectionConfig_Create();
 *
 *     if (ccnxConnectionConfig_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     ccnxConnectionConfig_Release(&a);
 *     ccnxConnectionConfig_Release(&b);
 * }
 * @endcode
 * @see ccnxConnectionConfig_HashCode
 */
bool ccnxConnectionConfig_Equals(const CCNxConnectionConfig *x, const CCNxConnectionConfig *y);


/**
 * Get the underlying JSON representation of a `CCNxConnectionConfig` instance.
 *
 * @param [in] config A pointer to a valid `CCNxConnectionConfig` instance.
 *
 * @return non-NULL A pointer to a valid PARCJSON instance.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCJSON *ccnxConnectionConfig_GetJson(const CCNxConnectionConfig *cssonfig);

/**
 * Add a component's configuration to the connection's configuration.  Each component snippit will
 * result in an addition like this:
 *
 * { "key" : { param1 : value1, param2 : value2, ... } }
 */
CCNxConnectionConfig *ccnxConnectionConfig_Add(CCNxConnectionConfig *connectionConfig, const char *key, PARCJSONValue *componentJson);

/**
 * Make a copy of the given CCNxConnectionConfig.  The original and copy
 * must both be destroyed.
 */
CCNxConnectionConfig *ccnxConnectionConfig_Copy(const CCNxConnectionConfig *original);

/**
 * Print a human readable representation of the given instance.
 *
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 * @param [in] instance A pointer to the instance to display.
 */
void ccnxConnectionConfig_Display(const CCNxConnectionConfig *instance, int indentation);

#endif
