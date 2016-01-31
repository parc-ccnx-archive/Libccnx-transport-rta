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
 * @file rta_CommandTransmitStatistics.h
 * @brief Represents a command to setup a statistics file
 *
 * Used to construct an RtaCommand object that is passed to rtaTransport_PassCommand() or _rtaTransport_SendCommandToFramework()
 * to send a command from the API's thread of execution to the Transport's thread of execution.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef Libccnx_rta_CommandTransmitStatistics_h
#define Libccnx_rta_CommandTransmitStatistics_h

struct rta_command_transmitstatistics;
typedef struct rta_command_transmitstatistics RtaCommandTransmitStatistics;

RtaCommandTransmitStatistics *rtaCommandTransmitStatistics_Create(struct timeval period, const char *filename);

/**
 * Increase the number of references to a `RtaCommandTransmitStatistics`.
 *
 * Note that new `RtaCommandTransmitStatistics` is not created,
 * only that the given `RtaCommandTransmitStatistics` reference count is incremented.
 * Discard the reference by invoking `rtaCommandTransmitStatistics_Release`.
 *
 * @param [in] transmitStats The RtaCommandTransmitStatistics to reference.
 *
 * @return non-null A reference to `transmitStats`.
 * @return null An error
 *
 * Example:
 * @code
 * {
 *    RtaCommandOpenConnection *transmitStats = rtaCommandTransmitStatistics_Create((struct timeval) { 1, 2 }, "filename");
 *    RtaCommandOpenConnection *second = rtaCommandTransmitStatistics_Acquire(transmitStats);
 *
 *    // release order does not matter
 *    rtaCommandTransmitStatistics_Release(&transmitStats);
 *    rtaCommandTransmitStatistics_Release(&second);
 * }
 * @endcode
 */
RtaCommandTransmitStatistics *rtaCommandTransmitStatistics_Acquire(const RtaCommandTransmitStatistics *transmitStats);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] openPtr A pointer to the object to release, will return NULL'd.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
void rtaCommandTransmitStatistics_Release(RtaCommandTransmitStatistics **openPtr);

/**
 * Returns the time period to use when writing statistics
 *
 * The time period is how often the transport will write the statistics to the specified file.
 *
 * @param [in] transmitStats An allocated RtaCommandTransmitStatistics
 *
 * @return timeval The value passed to rtaCommandTransmitStatistics_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     struct timeval period = { 1, 2 };
 *     const char *filename = "filename";
 *     RtaCommandOpenConnection *transmitStats = rtaCommandTransmitStatistics_Create(period, filename);
 *     struct timeval testValue = rtaCommandTransmitStatistics_GetPeriod(transmitStats);
 *     assertTrue(timercmp(&testValue, &period, ==), "Wrong period");
 *     rtaCommandTransmitStatistics_Release(&transmitStats);
 * }
 * @endcode
 */
struct timeval rtaCommandTransmitStatistics_GetPeriod(const RtaCommandTransmitStatistics *transmitStats);

/**
 * Returns the filename to use when writing statistics
 *
 * The filename to append statistics to.
 *
 * @param [in] transmitStats An allocated RtaCommandTransmitStatistics
 *
 * @return timeval The value passed to rtaCommandTransmitStatistics_Create().
 *
 * Example:
 * @code
 * {
 *     int stackId = 7;
 *     struct timeval period = { 1, 2 };
 *     const char *filename = "filename";
 *     RtaCommandOpenConnection *transmitStats = rtaCommandTransmitStatistics_Create(period, filename);
 *     struct timeval testValue = rtaCommandTransmitStatistics_GetPeriod(transmitStats);
 *     assertTrue(strcmp(filename, testValue) == 0, "Wrong filename");
 *     rtaCommandTransmitStatistics_Release(&transmitStats);
 * }
 * @endcode
 */
const char *rtaCommandTransmitStatistics_GetFilename(const RtaCommandTransmitStatistics *transmitStats);
#endif // Libccnx_rta_CommandTransmitStatistics_h
