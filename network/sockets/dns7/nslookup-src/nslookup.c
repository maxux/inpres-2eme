#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>
#include "nslookup.h"
#include "../common/physlib/physlib.h"

void diep(char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

char *ip_from_int(unsigned char *ip, char *buffer) {
	sprintf(buffer, "%u.%u.%u.%u", *ip, *(ip + 1), *(ip + 2), *(ip + 3));
	return buffer;
}

int socket_dns(char *server, int port, struct sockaddr_in *remote) {
	int sockfd;
	
	printf("[+] Connecting [%s:%d]...\n", server, port);
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		diep("socket");

	memset((char *) remote, 0, sizeof(struct sockaddr_in));
	
	remote->sin_family	= AF_INET;
	remote->sin_port	= htons(port);	
	remote->sin_addr.s_addr	= inet_addr(server);
	
	return sockfd;
}

/* Convert string with indexed dot size */
/* www.google.com => 3www6google3com */
unsigned char * fromhost(unsigned char *destination, char *hostname) {
	unsigned char *id = destination;
	unsigned char *write = destination + 1;
	unsigned char len = 0;
	
	while(*hostname) {
		if(*hostname == '.') {
			*id = len;
			id = write;
			len = 0;
			
		} else len++;
	
		*write = *hostname;
		
		hostname++;
		write++;
	}
	
	*write = '\0';
	*id = len;

	return destination;
}

unsigned int tohost(unsigned char *data, unsigned char *root, char *buffer) {
	unsigned int length = 0, total = 0;
	char *_buffer = buffer;
	
	while(*data && *data != 0xc0) {
		length = *data++;
		total += length + 1;
		
		/* Copy segment */
		strncpy(_buffer, (char*) data, length);
		
		/* Append dot */
		strcpy(_buffer + length, ".");
		
		data    += length;
		_buffer += length + 1;
	}
	
	/* 0xc0 -> pointer, recursive call to packet offset *(data + 1) */
	if(*data == 0xc0)
		total += tohost(root + *(data + 1), root, _buffer);
	
	return total + 1;	/* skip null byte */
}

void dump(unsigned char *data, unsigned int len) {
	unsigned int i;
	
	printf("[+] DATA DUMP\n");
	printf("[ ] 0x0000 == ");
	
	for(i = 0; i < len;) {
		printf("0x%02x ", data[i++]);
		
		if(i % 16 == 0)
			printf("\n[ ] 0x%04x == ", i);
	}
	
	printf("\n");
}

char * __dns_type(dns_type_t type) {
	if(type > (sizeof(__glob_dns_type_t) / sizeof(char*)))
		return __glob_dns_type_t[0];
	
	return __glob_dns_type_t[type];
}

char * __dns_class(dns_class_t class) {
	if(class > (sizeof(__glob_dns_class_t) / sizeof(char*)))
		return __glob_dns_class_t[0];
	
	return __glob_dns_class_t[class];
}

unsigned char * __parseresponse(unsigned char *request, unsigned int *length, unsigned char *root) {
	char ipv4[16];
	dns_rdata_t *rdata;
	unsigned char *_request = request;
	unsigned int _length = *length;
	char buffer[256];
		
	dump(_request, 30);
	
	// FIXME
	if(*_request != 0xc0) {
		fprintf(stderr, "[-] answer %x not supported yet\n", *_request);
		return NULL;
	}
	
	tohost(_request, root, buffer);
	printf("[+] Answer Name : %s\n", buffer);
	
	_request += 2;
	_length -= 2;
	
	dump(_request, 16);
	rdata = (dns_rdata_t*) _request;
	printf("[+] Answer Type : %d (%s)\n", ntohs(rdata->rtype), __dns_type(ntohs(rdata->rtype)));
	printf("[+] Answer Class: %d (%s)\n", ntohs(rdata->rclass), __dns_class(ntohs(rdata->rclass)));
	printf("[+] Answer TTL  : %d\n", ntohl(rdata->rttl));
	printf("[+] Answer Size : %d\n", ntohs(rdata->data_len));
	
	_request += sizeof(dns_rdata_t);
	_length  -= sizeof(dns_rdata_t);
	
	switch(ntohs(rdata->rtype)) {
		case NS:
		case CNAME:
		case PTR:
			tohost(_request, root, buffer);
			printf("[+] Answer Host : %s\n", buffer);
		break;
		
		case A:
			printf("[+] Answer Addr : %s\n", ip_from_int(_request, ipv4));
		break;
	}
	
	printf("\n");
	
	*length = _length;
	return _request + ntohs(rdata->data_len);
}

int __gethostbyname(char *name, resolv_conf_t *resolv, dns_question_t *oquest) {
	int sockfd;
	// struct sockaddr_in remote;
	
	unsigned char *request, *_request; //, *previous;
	dns_header_t header, *_header;
	dns_question_t question, *_question;
	
	unsigned int length = 0, _length = 0;
	char buffer[256];
	int i;
	
	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	
	/*
	if((sockfd = socket_dns(resolv->server, DNS_PORT, &remote)) == -1) {
		fprintf(stderr, "[-] Cannot create socket\n");
		exit(EXIT_FAILURE);
	}
	*/
	sockfd = CreateSockets(&psoo, &psos, "127.0.0.1", 10051, resolv->server, DNS_PORT);
	if(sockfd == -1)
		perror("CreateSockets") ;
	else
		fprintf(stderr, "CreateSockets %d\n", sockfd);
	
	
	/*
	 * Building Request
	 */
	request = (unsigned char*) malloc(sizeof(dns_header_t) + strlen(name) + 1 + sizeof(dns_question_t) + 2);
	
	/* Initialize empty request */
	bzero(&header, sizeof(dns_header_t));
	
	header.id	= rand() % 1337;	/* random id */
	header.qr	= QUERY;
	header.opcode	= STANDARD;
	// header.aa	= 0;
	// header.tc	= 0;		/* this is not truncated */
	header.rd	= 0x01;		/* enable recursion */
	// header.ra	= 0;
	// header.z	= 0;
	// header.ad	= 0;
	// header.cd	= 0;
	// header.rcode	= 0;
	
	header.q_count		= htons(1);	/* 1 question */
	// header.ans_count	= 0;
	// header.auth_count	= 0;
	// header.add_count	= 0;

	length = sizeof(dns_header_t);
	
	memcpy(request, &header, length);
	fromhost(request + length, name);
	
	question.qtype  = htons(oquest->qtype);
	question.qclass = htons(oquest->qclass);
	
	length += strlen(name) + 2;
	memcpy(request + length, &question, sizeof(dns_question_t));
	
	length += sizeof(dns_question_t) + 1;
	
	printf("[+] Sending Request: %u bytes\n", length);
	/*
	if(sendto(sockfd, request, length, 0, (const struct sockaddr *) &remote, sizeof(struct sockaddr_in)) == -1)
		fprintf(stderr, "[-] Cannot send datagram\n");
	*/
	if(SendDatagram(sockfd, request, length, &psos) != -1) {
		fprintf(stderr, "[+] Sent: xx bytes\n");
	
	} else perror("SendDatagram");
	
	/*
	 * Reading Response
	 */
	request = realloc(request, sizeof(char) * UDP_LIMIT);
	
	/*
	if((i = recv(sockfd, request, UDP_LIMIT, 0)) == -1)
		diep("recvfrom");
	*/
	if((i = ReceiveDatagram(sockfd, request, UDP_LIMIT, &psor)) == -1)
		return 1;
		
	length = (unsigned int) i;
	
	printf("[+] Response       : %u bytes\n", length);
	dump(request, length);
	
	_header = (dns_header_t*) request;
	printf("[+] Answers        : %d\n", ntohs(_header->ans_count));
	printf("[+] Authoritative  : %d\n", ntohs(_header->auth_count));
	printf("[+] Additional     : %d\n", ntohs(_header->add_count));
	
	if(_header->qr != RESPONSE) {
		fprintf(stderr, "[-] Reply is not a reponse packet (%d)\n", _header->qr);
		return 1;
	}
	
	_request = request + sizeof(dns_header_t);
	_length  = length - sizeof(dns_header_t);
	// dump(_request, _length);
	
	/*
	 * Pass 1: Copy of question
	 */
	printf("[_] QUESTIONS\n");
	printf("[_] #################################\n");
	for(i = 0; i < ntohs(_header->q_count); i++) {
		printf("[ ] Question #%d\n", i);
		dump(_request, 30);
		
		/* Extract name */
		printf("[ ] Question Name : ");
		length = tohost(_request, request, buffer);
		printf("[ ] Question Host : %s\n", buffer);
		
		_question = (dns_question_t*) (_request + length);
		printf("[ ] Question Type : %d\n", ntohs(_question->qtype));
		printf("[ ] Question Class: %d\n", ntohs(_question->qclass));
		
		/* Skip question */
		_request += length + sizeof(dns_question_t);
		_length  -= length + sizeof(dns_question_t);
	}
	
	printf("\n");
	
	/*
	 * Pass 2: Answers
	 */
	printf("[_] ANSWERS\n");
	printf("[_] #################################\n");
	for(i = 0; i < ntohs(_header->ans_count); i++) {
		printf("[ ] Answer #%d\n", i);
		if((_request = __parseresponse(_request, &_length, request)) == NULL)
			break;
		
		break;
	}
	
	goto end;
	
	/*
	 * Pass 3: Authoritative
	 */
	printf("[_] AUTHORITATIVE\n");
	printf("[_] #################################\n");
	for(i = 0; i < ntohs(_header->auth_count); i++) {
		printf("[ ] Authoritative #%d\n", i);
		if((_request = __parseresponse(_request, &_length, request)) == NULL)
			break;
	}
	
	/*
	 * Pass 4: Additional
	 */
	printf("[_] ADDITIONAL\n");
	printf("[_] #################################\n");
	for(i = 0; i < ntohs(_header->add_count); i++) {
		printf("[ ] Additional #%d\n\n", i);
		if((_request = __parseresponse(_request, &_length, request)) == NULL)
			break;
	}
	
	end:
	
	free(request);
	close(sockfd);
	
	return 0;
}

void init_client(resolv_conf_t *resolv) {
	FILE *fp;
	char buffer[256];
	size_t len;
	
	printf("[ ] Parser: init: %s\n", RESOLV_CONF);
	
	fp = fopen(RESOLV_CONF, "r");
	if(!fp)
		diep(RESOLV_CONF);
	
	while(fgets(buffer, sizeof(buffer), fp)) {
		if(!strncmp(buffer, "nameserver", 10)) {
			len = strlen(buffer + 10);
			resolv->server = (char*) malloc(sizeof(char) * len);
			
			strncpy(resolv->server, buffer + 11, len - 2);
			resolv->server[len - 1] = '\0';
			
			continue;
		}
		
		if(!strncmp(buffer, "search", 6)) {
			len = strlen(buffer + 6);
			resolv->search = (char*) malloc(sizeof(char) * len);
			
			strncpy(resolv->search, buffer + 7, len - 2);
			resolv->search[len - 1] = '\0';
			
			continue;
		}
	}
}

char * trim(char *str) {
	size_t len;
	int i;
	
	len = strlen(str);
	for(i = len; i > 0; i--)
		if(isspace(str[i]))
			str[i] = '\0';
	
	return str;
}

int main(void) {
	char buffer[256], host[256];
	int d1, d2, d3, d4;
	resolv_conf_t resolv;
	dns_question_t question = {
		.qclass = IN
	};
	
	/* Reading system files */
	init_client(&resolv);
	
	printf("[+] DNS Server: <%s>\n", resolv.server);
	printf("[+] DNS Search: <%s>\n", resolv.search);
	
	while(1) {
		printf("> ");
		fflush(stdout);
		
		if(!fgets(buffer, sizeof(buffer), stdin))
			diep("fgets");
		
		trim(buffer);
		printf("[+] Request: <%s>\n", buffer);
		
		if(isdigit(*buffer)) {
			if(sscanf(buffer, "%d.%d.%d.%d", &d1, &d2, &d3, &d4) == 4) {
				if(d1 < 256 && d2 < 256 && d3 < 256 && d4 < 256) {
					strcpy(host, buffer);
					printf("[ ] Request IP address\n");
					
					question.qtype = PTR;
					
				} else {
					printf("[-] Invalid IP address\n");
					continue;
				}
			}
			
		} else if(buffer[strlen(buffer) - 1] != '.') {
			printf("[ ] Not FQDN request, appending search\n");
			snprintf(host, sizeof(host), "%s.%s", buffer, resolv.search);
			
			question.qtype = A;
			
		} else {
			strncpy(host, buffer, strlen(buffer) - 1);
			host[strlen(buffer) - 1] = '\0';
			
			printf("[ ] FQDN Request\n");
			question.qtype = A;
		}
		
		printf("[+] Resolving <%s>...\n", host);
		__gethostbyname(host, &resolv, &question);
	}
	
	return 0;
}
