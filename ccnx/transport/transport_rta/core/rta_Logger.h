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
 * @file rta_Logger.h
 * @brief Logger for the Rta transport
 *
 * A facility based logger to allow selective logging from different parts of Rta
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef Rta_rta_Logger_h
#define Rta_rta_Logger_h

#include <sys/time.h>
#include <stdarg.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/logging/parc_LogLevel.h>
#include <parc/logging/parc_LogReporter.h>
#include <parc/algol/parc_Clock.h>

struct rta_logger;
typedef struct rta_logger RtaLogger;

/**
 * Framework - Overall framework
 * ApiConnector - API Connector
 * Flowcontrol - Flow controller
 * Codec - Codec and verification/signing
 * ForwarderConnector - Forwarder connector
 */
typedef enum {
    RtaLoggerFacility_Framework,
    RtaLoggerFacility_ApiConnector,
    RtaLoggerFacility_Flowcontrol,
    RtaLoggerFacility_Codec,
    RtaLoggerFacility_ForwarderConnector,
    RtaLoggerFacility_END          // sentinel value
} RtaLoggerFacility;

/**
 * Returns a string representation of a facility
 *
 * Do not free the returned value.
 *
 * @param [in] facility The facility to change to a string
 *
 * @retval string A string representation of the facility
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char * rtaLogger_FacilityString(RtaLoggerFacility facility);

/**
 * Returns a string representation of a log level
 *
 * Do not free the returned value.
 *
 * @param [in] level The level to change to a string
 *
 * @retval string A string representation of the level
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const char * rtaLogger_LevelString(PARCLogLevel level);

/**
 * Create a logger that uses a given writer and clock
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] writer The output writer
 * @param [in] clock The clock to use for log messages
 *
 * @retval non-null An allocated logger
 * @retval null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaLogger * rtaLogger_Create(PARCLogReporter *reporter, const PARCClock *clock);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @retval <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaLogger_Release(RtaLogger **loggerPtr);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in out in,out#>] <#name#> <#description#>
 *
 * @retval <#value#> <#explanation#>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaLogger *rtaLogger_Acquire(const RtaLogger *logger);

/**
 * Sets the minimum log level for a facility
 *
 * The default log level is ERROR.  For a message to be logged, it must be of equal
 * or higher log level.
 *
 * @param [in] logger An allocated logger
 * @param [in] facility The facility to set the log level for
 * @param [in] The minimum level to log
 *
 * @retval <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *    PARCLogReporter *reporter = parcLogReporterTextStdout_Create();
 *    RtaLogger *logger = rtaLogger_Create(reporter, parcClock_Wallclock());
 *    parcLogReporter_Release(&reporter);
 *    rtaLogger_SetLogLevel(logger, RtaLoggerFacility_IO, PARCLogLevel_Warning);
 * }
 * @endcode
 */
void rtaLogger_SetLogLevel(RtaLogger *logger, RtaLoggerFacility facility, PARCLogLevel minimumLevel);

/**
 * Tests if the log level would be logged
 *
 * If the facility would log the given level, returns true.  May be used as a
 * guard around expensive logging functions.
 *
 * @param [in] logger An allocated logger
 * @param [in] facility The facility to test
 * @param [in] The level to test
 *
 * @retval true The given facility would log the given level
 * @retval false A message of the given level would not be logged
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool rtaLogger_IsLoggable(const RtaLogger *logger, RtaLoggerFacility facility, PARCLogLevel level);

/**
 * Log a message
 *
 * The message will only be logged if it is loggable (rtaLogger_IsLoggable returns true).
 *
 * @param [in] logger An allocated RtaLogger
 * @param [in] facility The facility to log under
 * @param [in] level The log level of the message
 * @param [in] module The specific module logging the message
 * @param [in] format The message with varargs
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaLogger_Log(RtaLogger *logger, RtaLoggerFacility facility, PARCLogLevel level, const char *module, const char *format, ...);

/**
 * Switch the logger to a new reporter
 *
 * Will close the old reporter and re-setup the internal loggers to use the new reporter.
 * All current log level settings are preserved.
 *
 * @param [in] logger An allocated RtaLogger
 * @param [in] reporter An allocated PARCLogReporter
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaLogger_SetReporter(RtaLogger *logger, PARCLogReporter *reporter);

/**
 * Set a new clock to use with the logger
 *
 * The logger will start getting the time (logged as the messageid) from the specified clock
 *
 * @param [in] logger An allocated RtaLogger
 * @param [in] clock An allocated PARCClock
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void rtaLogger_SetClock(RtaLogger *logger, PARCClock *clock);
#endif // Rta_rta_Logger_h
