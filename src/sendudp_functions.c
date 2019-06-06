/*
 * sendudp_functions.c
 *
 *  Created on: 26 kwi 2019
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

void hexdump(unsigned char *data, unsigned int data_bytes) {

	int bin_p = 0;

	while (bin_p < data_bytes) {
		int j;
		for (j = 0; j < 8 && bin_p < data_bytes; j++) {
			printf("%02x ", data[bin_p++]);
		}

		printf("\n");
	}
}

int validate_number(char *number) {
	while (*number) {
		if (!isdigit(*number)) {
			return 0;
		}
		number = number + 1;
	}
	return 1;
}



