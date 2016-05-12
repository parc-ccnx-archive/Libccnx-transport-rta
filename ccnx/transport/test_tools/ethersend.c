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
 * Send a file as the payload of an ethernet frame
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
////
// Using libpcap because it works on mac and linux.

/*
 This is experimental code to look at storing CCNxHF content via
 AoE and retrieving it directly from the CCNxHF interest hashes.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <assert.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <netinet/in.h>

typedef uint8_t u_char;
typedef uint16_t u_short;
typedef uint32_t u_int;


#include <pcap.h>

#define ETHERTYPE 0x0801

struct _packet {
    u_int8_t    dst[6];
    u_int8_t    src[6];
    u_int16_t   ethertype;
    u_int8_t    data[0];
} __attribute__((packed));

// ====================
// our data structures
//

static void send_file(pcap_t *handle, uint8_t smac[], uint8_t dmac[], const char *filename);


// ====================

static
void printhex(u_int8_t *buffer, int length)
{
    int i;
    for(i = 0; i < length; i++)
        printf("%02X", buffer[i]);
}



static void
send_file(pcap_t *handle, uint8_t smac[], uint8_t dmac[], const char *filename)
{
    struct stat statbuf;

    struct _packet *packet = malloc(1500);
    memset(packet, 0, 1500);

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file: ");
        abort();
    }

    if (fstat (fd,&statbuf) < 0) {
        perror ("fstat error");
        abort();
    }

    uint8_t *src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (src == MAP_FAILED) {
        perror ("mmap error");
        abort();
    }

    assert(statbuf.st_size <= 1500);

    size_t len = sizeof(struct _packet) + statbuf.st_size;
    printf("Sending config/query size %zu\n", len);

    memcpy(packet->dst, dmac, 6);
    memcpy(packet->src, smac, 6);

    packet->ethertype = htons(ETHERTYPE);

    memcpy(packet->data, src, statbuf.st_size);

    int x = pcap_inject(handle, packet, len);
    printf("%s wrote %d bytes\n", __func__, x);

    free(packet);
}

static
void get_mac_address(const char *deviceName, u_int8_t *mac)
{
    struct ifaddrs *ifap;
    struct ifaddrs *next;

    int x = getifaddrs(&ifap);
    if( x != 0 ) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    next = ifap;
    while( next != NULL )
    {
#if defined(__APPLE__)
        if( strstr(deviceName, next->ifa_name) != NULL && next->ifa_addr->sa_family == AF_LINK )
#elif defined(__linux__)
        if( strstr(deviceName, next->ifa_name) != NULL && next->ifa_addr->sa_family == AF_PACKET )
#else
#error Unsupported platform
#endif
        {
/*
                        printf("name %s family: %d len %u: ",
                               next->ifa_name,
                               next->ifa_addr->sa_family,
                               next->ifa_addr->sa_len);
                        printhex(next->ifa_addr->sa_data, next->ifa_addr->sa_len);
                        printf("\n");
 */
            memcpy(mac, next->ifa_addr->sa_data + 9, 6);
            break;
        }
        next = next->ifa_next;
    }
    freeifaddrs(ifap);
}

static void
macStringToArray(const char *string, size_t outputLength, uint8_t output[])
{
    assert(outputLength == 6);

    sscanf(string, "%02x:%02x:%02x:%02x:%02x:%02x", &output[0], &output[1], &output[2], &output[3], &output[4], &output[5]);
}

int main(int argc, const char * argv[])
{
    if (argc != 4 || argv[1][0] == '-') {
        printf("usage: ethersend dev dst filename\n");
        printf("\n");
        printf("Will send filename as the payload of an ethernet frame to dst\n");
        printf("\n");
        printf("example: ethersend eth0 a8:20:66:3b:30:bc interest.bin\n");
        printf("\n");
        exit(EXIT_FAILURE);
    }

    pcap_t *handle;			/* Session handle */
    const char *dev = argv[1];			/* The device to sniff on */
    char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
    struct bpf_program fp;		/* The compiled filter */
    char filter_exp[1024];	/* The filter expression */
    bpf_u_int32 mask;		/* Our netmask */
    bpf_u_int32 net;		/* Our IP */

    sprintf(filter_exp, "ether proto 0x%04X", ETHERTYPE);

/*
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return(2);
    }
*/
	printf("dev = %s\n", dev);

    /* Find the properties for the device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    u_int8_t mymac[6];
    get_mac_address(dev, mymac);
    printf("My mac address: "); printhex(mymac, 6); printf("\n");

    u_int8_t dmac[6];
    macStringToArray(argv[2], 6, dmac);
    printf("dmac address  : "); printhex(dmac, 6); printf("\n");


    /* Open the session in promiscuous mode */
    handle = pcap_open_live(dev, 1500, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return(2);
    }

    /* Compile and apply the filter */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }

    send_file(handle, mymac, dmac, argv[3]);


    //    /* Grab a packet */
    //    packet = pcap_next(handle, &header);
    //    /* Print its length */
    //    printf("Jacked a packet with length of [%d]\n", header.len);
    /* And close the session */
    pcap_close(handle);
    return(0);
}

//static
//void print_buffer(u_int8_t *buffer, int length)
//{
//	int i;
//
//    for(i = 0; i < length; i++)
//    {
//        if( (i % 16) == 0 )
//            printf("\n%4d : ", i);
//
//        printf("%02X", buffer[i]);
//
//        if( ((i+1) % 4) == 0 )
//            printf(" ");
//    }
//    printf("\n");
//}
