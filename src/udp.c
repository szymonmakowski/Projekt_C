/*
 * udp.c
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

//Function which calculates checksum of UDP datagram
uint16_t checksum(uint8_t *data, unsigned int size) {
	int i;
	int sum = 0;
	uint16_t *p = (uint16_t *) data;

	for (i = 0; i < size; i += 2) {
		sum += *(p++);
	}

	uint16_t carry = sum >> 16;
	uint16_t tmp = 0x0000ffff & sum;
	uint16_t res = ~(tmp + carry);

	return res;
}

unsigned int build_udp_packet(struct sockaddr_in src_addr,
		struct sockaddr_in dst_addr, uint8_t *udp_packet, uint8_t *data,
		unsigned int data_size) {

	uint8_t pseudo_packet[MAX_PSEUDO_PKT_SIZE];
	uint16_t length;
	struct udphdr *udph;
	struct pseudo_iphdr *p_iphdr = (struct pseudo_iphdr *) pseudo_packet;

	length = UDP_HDR_SIZE + data_size;
	udph = (struct udphdr *) udp_packet;

	udph->source = src_addr.sin_port;
	udph->dest = dst_addr.sin_port;
	udph->len = htons(length);

	memcpy(udp_packet + UDP_HDR_SIZE, data, data_size);

	if (length + sizeof(struct pseudo_iphdr) > MAX_PSEUDO_PKT_SIZE) {
		fprintf(stderr, "Buffer size not enough");
		exit(1);
	}

	// Calculate checksum with pseudo ip header

	p_iphdr->source_addr = src_addr.sin_addr.s_addr;
	p_iphdr->dest_addr = dst_addr.sin_addr.s_addr;
	p_iphdr->zeros = 0;
	p_iphdr->prot = IPPROTO_UDP; //udp
	p_iphdr->length = udph->len;

	// Do NOT use udph->len instead of length.
	// udph->len is in big endian

	memcpy(pseudo_packet + sizeof(struct pseudo_iphdr), udph, length);
	udph->check = checksum(pseudo_packet, sizeof(struct pseudo_iphdr) + length);

	return length;
}

