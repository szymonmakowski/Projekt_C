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

#include "SendUDP.h"

typedef struct lw {
	struct lw *pierwszy;
	struct lw *nastepny;
	struct lw *poprzedni;
	uint8_t dane[ETH_DATA_LEN];
};

struct lw *nowy = NULL;
struct lw *pierwszy = NULL;
struct lw *poprzedni = NULL;

void dodaj_elementy(uint8_t* packet, unsigned int packet_size, int number_of_packets) {
	for (int i = 0; i < number_of_packets; i++) {
		if (!pierwszy) {

			nowy = (struct lw *) malloc(sizeof(struct lw));
			pierwszy = nowy;
			nowy->pierwszy = nowy;
			nowy->poprzedni = NULL;
			nowy->nastepny = NULL;
			memcpy(nowy->dane, packet, packet_size);
			poprzedni = nowy;
		} else {
			nowy = (struct lw *) malloc(sizeof(struct lw));
			poprzedni->nastepny = nowy;
			nowy->pierwszy = pierwszy;
			nowy->nastepny = NULL;
			nowy->poprzedni = poprzedni;
			memcpy(nowy->dane, packet, packet_size);
			poprzedni = nowy;
		}
	}
}
void wyswietl(unsigned int packet_size) {

	for (struct lw* tmp = pierwszy; tmp != NULL; tmp = tmp->nastepny) {
		int bin_p = 0;

		while (bin_p < packet_size) {
			int j;
			for (j = 0; j < 8 && bin_p < packet_size; j++) {
				printf("%02x ", (tmp->dane)[bin_p++]);
			}

			printf("\n");
		}
	}

}
void usun_element(unsigned int ktory, int number_of_packets) {

	if (!(ktory > number_of_packets)) {
		struct lw * wsk = pierwszy;
		struct lw * pom;

		for (int i = 0; i < ktory - 1; i++)
			wsk = wsk->nastepny;

		if ((pom = wsk->poprzedni)) {
			pom->nastepny = wsk->nastepny;
			if ((pom = wsk->nastepny)) {
				pom->poprzedni = wsk->poprzedni;
				free(wsk);
			} else
				free(wsk);
		} else {
			pom = wsk->nastepny;
			pom->poprzedni = NULL;
			pierwszy = pom;
			free(wsk);
			for (struct lw* tmp = pierwszy; tmp != NULL; tmp = tmp->nastepny)
				tmp->pierwszy = pierwszy;
		}
	} else
		printf("Nie ma takiego elementu na liscie");
}

void send_udp_packet(int raw_sock, struct sockaddr_in src_addr,
		struct sockaddr_in dst_addr, unsigned int packet_size) {

	for (struct lw* tmp = pierwszy; tmp != NULL; tmp = tmp->nastepny) {
		if (sendto(raw_sock, tmp->dane, packet_size, 0,
				(struct sockaddr *) &dst_addr, sizeof(dst_addr)) < 0) {
			perror("sendto");
			exit(1);
		}
	}
}
