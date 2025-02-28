/*
 ============================================================================
 Name        : SendUDP.c
 Author      : Szymon Makowski
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
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

int main(int argc, char *argv[]) {

	int raw_sock;
	int options;
	int opt_number;
	int interface_set_flag = 0;
	int number_of_packets = 1;
	int ip_src_flag = 0;
	int ip_dst_flag = 0;
	int port_src_flag = 0;
	int port_dst_flag = 0;
	int data_flag = 0;
	uint16_t dstport = 4086; // Default destination port
	uint16_t srcport = 2048; // Default source port
	uint8_t type_of_service = 0; // Default TOS
	uint8_t ttl = 64; // Default TTL
	uint8_t packet[ETH_DATA_LEN];
	uint8_t udp_packet[ETH_DATA_LEN];
	uint8_t data[MAX_DATA_SIZE];
	char ifName[IFNAMSIZ];
	char opt[] = { 's', 'd', 'p', 'x', 'i', 'n', 't', 'l', 'm','h' };
	char *sending_data = "SEND UDP"; // Default data to send
	char *dstadr = "127.0.0.1"; // Default destination adress
	char *srcadr = "192.168.1.1"; // Default source adress
	unsigned int packet_size;
	unsigned int data_size;
	struct sockaddr_in src_addr;
	struct sockaddr_in dst_addr;
	struct ifreq ifr;

	/*********************SOCKET-**********************/

	if ((raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("socket");
		exit(1);
	}

/********************************GETOPT-pobieranie parametrów z terminala******************************/
	while ((options = getopt(argc, argv, "s:d:p:x:i:n:t:l:m:h")) != -1) {
		switch (options) {
		case 's':
			if (inet_pton(AF_INET, optarg, &(src_addr.sin_addr)) <= 0) {
				printf("ERROR, IPv4 adress is not correct !\n");
				exit(1);
			} else {
				ip_src_flag = 1;
				srcadr = optarg;
			}
			break;
		case 'd':
			if (inet_pton(AF_INET, optarg, &(dst_addr.sin_addr)) <= 0) {
				printf("ERROR, IPv4 adress is not correct !\n");
				exit(1);
			} else {
				ip_dst_flag = 1;
				dstadr = optarg;
			}
			break;
		case 'p':
			if (validate_number(optarg) == 1) {
				srcport = (uint16_t) atoi(optarg);
				src_addr.sin_port = htons(srcport);
				port_src_flag = 1;
			} else {
				printf("ERROR, Port number is not correct !\n");
				exit(1);
			}
			break;
		case 'x':
			if (validate_number(optarg) == 1) {
				dstport = (uint16_t) atoi(optarg);
				dst_addr.sin_port = htons(dstport);
				port_dst_flag = 1;
			} else {
				printf("ERROR, Port number is not correct !\n");
				exit(1);
			}
			break;
		case 'i':
			strcpy(ifName, optarg);
			interface_set_flag = 1;
			break;
		case 'n':
			number_of_packets = atoi(optarg);
			break;
		case 't':
			if ((validate_number(optarg)) == 1 && (atoi(optarg) < 256)
					&& (atoi(optarg) >= 0)) {
				type_of_service = (uint8_t) atoi(optarg);
			} else {
				printf("ERROR, TOS number is not correct !\n");
				exit(1);
			}
			break;
		case 'l':
			if ((validate_number(optarg) == 1) && (atoi(optarg) < 256)
					&& (atoi(optarg) > 0)) {
				ttl = (uint8_t) atoi(optarg);
			} else {
				printf("ERROR, TTL number is not correct !\n");
				exit(1);
			}
			break;
		case 'm':
			sending_data = optarg;
			data_flag = 1;
			break;
		case 'h':
			printf("-s <source ip adress> -d <destination ip adress> -p <source port> -x <destination port>\n");
			printf("-i <interface name> -n <number of packets> -t <type of service> -l <time to live> -m <message> -h <help>\n");
			exit(1);
			break;
		case '?':
			for (opt_number = 0; opt_number < 10; opt_number++) {
				if (optopt == opt[opt_number]) {
					fprintf(stderr, "Option -%c requires an argument.\n",
							optopt);
					exit(1);
				}
			}
			if (isprint(optopt)) {
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				exit(1);
			}
		}
	}

	if (interface_set_flag == 0) {
		printf("[+] Default interface %s...\n\n", DEFAULT_IF);
		strcpy(ifName, DEFAULT_IF);
	}
	if (ip_src_flag == 0) {
		printf("[+] Default source address %s...\n\n", srcadr);
		inet_pton(AF_INET, srcadr, &(src_addr.sin_addr));
	}
	if (ip_dst_flag == 0) {
		printf("[+] Default destination address %s...\n\n", dstadr);
		inet_pton(AF_INET, dstadr, &(dst_addr.sin_addr));
	}
	if (port_src_flag == 0) {
		printf("[+] Default source port %d...\n\n", srcport);
		src_addr.sin_port = htons(srcport);
	}
	if (port_dst_flag == 0) {
		printf("[+] Default destination port %d...\n\n", dstport);
		dst_addr.sin_port = htons(dstport);
	}
	if (data_flag == 0) {
		printf("[+] Default data to send \"%s\"...\n\n", sending_data);
	}

	/**********************************************************************************/

	strcpy((char *) data, sending_data);
	data_size = strlen(sending_data);



	printf("[+] Build UDP packet...\n\n");

	open_udp();
	packet_size = buildudp(src_addr, dst_addr, udp_packet, data,
			data_size);
	close_udp();

	hexdump(udp_packet, packet_size);
	printf("\n\n");


	printf("[+] Build IP packet...\n\n");

	open_ipv4();
	packet_size = buildip(src_addr.sin_addr, dst_addr.sin_addr,
	IPPROTO_UDP, type_of_service, ttl, packet, udp_packet, packet_size);
	close_ipv4();

	hexdump(packet, packet_size);
	printf("\n\n");

	memset(&ifr, 0, sizeof(ifr));
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), ifName);
	if (setsockopt(raw_sock, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr,
			sizeof(ifr)) < 0) {
		perror("SIOCGIFINDEX: SET CORRECT INTERFACE NAME");
		exit(1);
	}

	open_list();
	//Add packets to list
	printf("[+] Add [%d] UDP datagram(-s) to list...\n", number_of_packets);

	add_packet(packet, packet_size, number_of_packets);

	//Print list
	printf("[+] Print [%d] element(-s) list...\n", number_of_packets);

	print_packet(packet_size);

	//Send datagram-s from list
	printf("[+] Send [%d] UDP datagram(-s) from list...\n", number_of_packets);

	sendudp(raw_sock, src_addr, dst_addr, packet_size);

	printf("[+] %s ---> %s\n", srcadr, dstadr);

	printf("[+] Clear list...\n");
	//Clear list
	for (int i = 1; i < number_of_packets;i++) {
		delete_packet(i,number_of_packets);
	}

	close_list();

	return 0;
}
