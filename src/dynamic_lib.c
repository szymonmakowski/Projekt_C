/*
 * dynamic_lib.c
 *
 *  Created on: 6 cze 2019
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

void open_ipv4() {

	ipv4 = dlopen("/home/szymon/eclipse-workspace/SendUDP/src/ipv4.so",
	RTLD_LAZY);

	if (!ipv4) {
		printf("ERROR WHILE OPENING: %s\n", dlerror());
		exit(1);
	}

	buildip = dlsym(ipv4, "build_ip_packet");

	if (ipv4 == NULL) {
		printf("Function 'build_ip_packet' not found in library");
	}

}

void close_ipv4() {

	if (dlclose(ipv4)) {

		printf("ERROR WHILE CLOSING: %s", dlerror());
		exit(1);
	}

}

void open_udp() {

	udp = dlopen("/home/szymon/eclipse-workspace/SendUDP/src/udp.so",
	RTLD_LAZY);

	if (!udp) {
		printf("ERROR WHILE OPENING: %s\n", dlerror());
		exit(1);
	}

	buildudp = dlsym(udp, "build_udp_packet");

	if (udp == NULL) {
		printf("Function 'build_udp_packet' not found in library");
	}

}

void close_udp() {

	if (dlclose(udp)) {

		printf("ERROR WHILE CLOSING: %s", dlerror());
		exit(1);
	}
}

void open_list() {

	list = dlopen("/home/szymon/eclipse-workspace/SendUDP/src/lista.so",
	RTLD_LAZY);

	if (!list) {
		printf("ERROR WHILE OPENING: %s\n", dlerror());
		exit(1);
	}

	sendudp = dlsym(list, "send_udp_packet");

	if (list == NULL) {
		printf("Function 'send_udp_packet' not found in library");
	}

	add_packet = dlsym(list, "dodaj_elementy");

	if (list == NULL) {
		printf("Function 'dodaj_elementy' not found in library");
	}

	print_packet = dlsym(list,"wyswietl");

	if (list == NULL) {
		printf("Function 'wyswietl' not found in library");
	}

	delete_packet = dlsym(list,"usun_element");

	if (list == NULL) {
		printf("Function 'usun_element' not found in library");
	}

}

void close_list() {

	if (dlclose(list)) {

		printf("ERROR WHILE CLOSING: %s", dlerror());
		exit(1);

	}
}

