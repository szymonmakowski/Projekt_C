/*
 * ipv4.c
 *
 *  Created on: 30 kwi 2019
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <getopt.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <dlfcn.h>

#include "SendUDP.h"

unsigned int build_ip_packet(struct in_addr src_addr, struct in_addr dst_addr,
		uint8_t protocol, uint8_t type_of_service, uint8_t ttl,
		uint8_t *ip_packet, uint8_t *data, unsigned int data_size) {

	struct iphdr *ip_header;

	ip_header = (struct iphdr *) ip_packet;

	ip_header->version = 4; //Only IPv4
	ip_header->ihl = INET_HDR_LEN; // Filled by default macro expression INET_HDR_LEN
	ip_header->tos = type_of_service; // Filled by getopt parameter -t or by default
	ip_header->tot_len = htons(INET_HDR_LEN * 4 + data_size); // Filled by default macro expression INET_HDR_LEN
	ip_header->id = 0; // Filled in automatically
	ip_header->frag_off = 0; //Filled in automatically
	ip_header->ttl = ttl; //Filled by getopt parameter -l or by default 64
	ip_header->protocol = protocol; //Only UDP
	ip_header->check = 0; // Filled in automatically
	ip_header->saddr = src_addr.s_addr; // Filled by getopt -s parameter or by default
	ip_header->daddr = dst_addr.s_addr; //Filled by getopt -d parameter or by default

	memcpy(ip_packet + sizeof(struct iphdr), data, data_size);

	return sizeof(struct iphdr) + data_size;
}
