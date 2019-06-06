/*
 * SendUDP.h
 *
 *  Created on: 26 kwi 2019
 *      Author: root
 */

#ifndef SENDUDP_H_
#define SENDUDP_H_

#define MAX_DATA_SIZE 1024
#define UDP_HDR_SIZE 8
#define INET_HDR_LEN 5
#define DEFAULT_IF	"eth0"
#define MAX_PSEUDO_PKT_SIZE 1024

struct pseudo_iphdr {
	uint32_t source_addr;
	uint32_t dest_addr;
	uint8_t zeros;
	uint8_t prot;
	uint16_t length;
};

//Biblioteki dynamiczne
void *ipv4, *udp, *list, *sendudp_functions;
//Wskaźniki na poszczególne funkcje
//void (*hex)(unsigned char*, unsigned int);
//int (*validate)(char*);
void (*sendudp)(int, struct sockaddr_in, struct sockaddr_in, unsigned int);
void (*add_packet)(uint8_t*, unsigned int, int);
void (*print_packet)(unsigned int);
void (*delete_packet)(unsigned int, int);
unsigned int (*buildudp)(struct sockaddr_in, struct sockaddr_in, uint8_t*,
		uint8_t*, unsigned int);
unsigned int (*buildip)(struct in_addr, struct in_addr, uint8_t, uint8_t,
		uint8_t, uint8_t*, uint8_t*, unsigned int);
//Otwieranie i zamykanie bibliotek dynamicznych
void open_ipv4();
void close_ipv4();
void open_udp();
void close_udp();
void open_list();
void close_list();
//void open_sendudp_functions();
//void close_sendudp_functions();

void hexdump(unsigned char *data, unsigned int data_bytes);
int validate_number(char *number);


/*
void dodaj_elementy(uint8_t*, unsigned int,int);

void wyswietl(unsigned int);

void usun_element(unsigned int,int);

int validate_number(char *number);

unsigned int build_udp_packet(struct sockaddr_in src_addr,
		struct sockaddr_in dst_addr, uint8_t *udp_packet, uint8_t *data,
		unsigned int data_size);

unsigned int build_ip_packet(struct in_addr src_addr, struct in_addr dst_addr,
		uint8_t protocol, uint8_t type_of_service, uint8_t ttl,
		uint8_t *ip_packet, uint8_t *data, unsigned int data_size);

void send_udp_packet(int raw_sock, struct sockaddr_in src_addr,
		struct sockaddr_in dst_addr, unsigned int packet_size);

void hexdump(unsigned char *data, unsigned int data_bytes);
*/
#endif /* SENDUDP_H_ */
