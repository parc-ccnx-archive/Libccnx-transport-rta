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
 * Utility function to write the test data to data files.  The data files will be in a format
 * you can use to import with "text2pcap".  For example:
 *
 *     text2pcap -u 9695,9695 file
 *
 * would add a fake UPD/IP/Ethernet header with UDP ports 9695 for source and destination
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <stdio.h>

#include <ccnx/common/codec/schema_v0/testdata/testrig_truthSet.h>
#include <ccnx/common/codec/schema_v1/testdata/v1_testrig_truthSet.h>

/*
 * typedef struct testrig_truth_table
 * {
 * const char *      testname;
 * uint8_t *         packet;
 * size_t            length;
 *
 * TlvErrorCodes     expectedError;
 *
 * // the array is terminated by a T_INVALID value
 * // for "arrayIndexOrTypeKey"
 * TruthTableEntry * entry;
 * } TruthTable;
 *
 */

static void
writePacket(TruthTable *table)
{
    char filename[1024];
    snprintf(filename, 1024, "%s.txt", table->testname);
    FILE *fh = fopen(filename, "w+");
    printf("name %s\n", filename);

    int linewidth = 8;
    for (int i = 0; i < table->length; i++) {
        if ((i % linewidth) == 0) {
            fprintf(fh, "\n%06X  ", i);
        }
        fprintf(fh, "%02X ", table->packet[i]);
    }
    fprintf(fh, "\n");
    fclose(fh);
}

static void
loopTruthTable(TruthTable truthset[])
{
    for(int i = 0; truthset[i].packet != NULL; i++)
    {
        writePacket(&truthset[i]);
    }

}

int main(int argc, char **argv)
{
    loopTruthTable(interests_truthSet);
    loopTruthTable(contentObject_truthSet);
    loopTruthTable(cpi_truthSet);

    loopTruthTable(v1_interests_truthSet);
    loopTruthTable(v1_contentObject_truthSet);
    loopTruthTable(v1_cpi_truthSet);
}
