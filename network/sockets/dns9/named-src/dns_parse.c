#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dns.h"
#include "named.h"
#include "parser.h"
#include "../common/physlib/physlib.h"
#include "dns_parse.h"

char *__pkt;
size_t __pkt_size;
int __sockfd;
struct sockaddr_in *__psor;

char *__glob_dns_opcode_t[] = {"STANDARD"};
char *__glob_dns_query_t[] = {"QUERY", "RESPONSE"};
char *__glob_dns_type_t[] = {"UNKNOWN", "A", "NS", "MD", "MF", "CNAME", "SOA", "MB", "MG", "MR", "NULL", "WKS", "PTR", "HINFO", "MINFO", "MX", "TXT"};
char *__glob_dns_class_t[] = {"UNKNOWN", "IN", "CS", "CH", "HS"};

char * __dns_opcode(dns_opcode_t opcode) {
	if(opcode > (sizeof(__glob_dns_opcode_t) / sizeof(char*)))
		return "INVALID";
	
	return __glob_dns_opcode_t[opcode];
}

char * __dns_qr(dns_query_t query) {
	if(query > (sizeof(__glob_dns_query_t) / sizeof(char*)))
		return "INVALID";
	
	return __glob_dns_query_t[query];
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


domaine_md_t * search_domaine(domaine_md_t *root, char *name) {
	int len;
	
	while(root) {
		/*
		 * query   = <hello.|maxux.|net.>
		 * (1) len = <      |      |   ^> (+ strlen(name))
		 * (2) len = <      | ^    |    > (- strlen(root->nom))
		 * (3) len = <      |^     |    > (-1)
		 * db      = <      |maxux.|net >
		 */
		len = strlen(name) - strlen(root->nom) - 1;
		
		// printf("%s <> %s\n", root->nom, name + len);
			
		if(len >= 0 && !strncmp(root->nom, name + len, strlen(root->nom)))
			return root;

		root = root->next;
	}
	
	return NULL;
}

netid_md_t * search_netid(netid_md_t *root, char *netid) {
	int o1, o2, o3, o4;
	
	if(sscanf(netid, "%d.%d.%d.%d", &o1, &o2, &o3, &o4) != 4) {
		fprintf(stderr, "[-] DNS Search: malformed IP\n");
		return NULL;
	}
	
	while(root) {
		if(root->o1 == o1 && (root->o2 == o2 || !root->o2) &&
		  (root->o3 == o3 || !root->o3) && (root->o4 == o4 || !root->o4))
			  return root;
		
		root = root->next;
	}
	
	return NULL;
}

records_md_t * dns_delegate(char *server, char *name) {
	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	int sockfd, rc, i;
	char buffer[2048];
	
	sockfd = CreateSockets(&psoo, &psos, "127.0.0.1", 10030, server, 10053);
	if(sockfd == -1)
		perror("CreateSockets") ;
	else
		fprintf(stderr, "CreateSockets %d\n", sockfd);
	
	if((rc = SendDatagram(sockfd, __pkt, __pkt_size, &psos)) == -1)
		perror("SendDatagram:");
		
	else printf("[+] Answer: sent: %d bytes\n", rc);
	
	if((i = ReceiveDatagram(sockfd, buffer, sizeof(buffer), &psor)) == -1)
		return 1;
	
	if((rc = SendDatagram(__sockfd, buffer, i, __psor)) == -1)
		perror("SendDatagram:");
		
	else printf("[+] Answer: sent: %d bytes\n", i);
	
	close(sockfd);
	
	return 1;
}

records_md_t * search_record(domaine_md_t *domain, dns_type_t type, char *name) {
	domaine_md_t *start = domain;
	size_t i;
	records_md_t *tmp;
	char *ptr;
	
	for(i = 0; i < domain->nbrecord; i++) {
		if(!strcmp(domain->record[i].gauche, name)) {
			if(domain->record[i].type == type) {
				return (domain->record + i);
				
			} else if(domain->record[i].type == CNAME) {
				printf("[+] Record found as CNAME, resolving...\n");
				printf("[+] Record: %s/%s/%s. Resolving...\n", domain->record[i].gauche, __dns_type(domain->record[i].type), domain->record[i].droite);
				return search_record(start, A, domain->record[i].droite);
				
			} else if(domain->record[i].type == NS && !strcmp(domain->record[i].gauche, name)) {
				printf("[+] Record found as NS, delegating...\n");
				printf("[+] Record: %s/%s/%s. Forwarding...\n", domain->record[i].gauche, __dns_type(domain->record[i].type), domain->record[i].droite);
				
				tmp = search_record(start, A, domain->record[i].droite);
				return dns_delegate(tmp->droite, name);
			}
		}
	}
	
	ptr = name;
	
	// Fallback to NS
	while((ptr = strstr(ptr, ".") + 1)) {
		for(i = 0; i < domain->nbrecord; i++) {
			if(domain->record[i].type == NS && !strcmp(domain->record[i].gauche, ptr)) {
				// Skip himself recursion
				if(!strcmp(domain->record[i].gauche, domain->record[i].droite))
					return NULL;
				
				printf("[+] Record found as NS, delegating...\n");
				printf("[+] Record: %s/%s/%s. Forwarding...\n", domain->record[i].gauche, __dns_type(domain->record[i].type), domain->record[i].droite);
				
				tmp = search_record(start, A, domain->record[i].droite);
				return dns_delegate(tmp->droite, name);
			}
		}
	}
	
	return NULL;
}

records_md_t * search_record_ptr(netid_md_t *domain, char *name) {
	size_t i;
	char temp[64];
	int o1, o2, o3, o4;
	
	if(sscanf(name, "%d.%d.%d.%d", &o1, &o2, &o3, &o4) != 4) {
		fprintf(stderr, "[-] Record: Malformed IP\n");
		return NULL;
	}
	
	sprintf(temp, "%d.%d.%d.%d.in-addr.arpa", o4, o3, o2, o1);
	
	for(i = 0; i < domain->nbrecord; i++) {
		if(!strcmp(domain->record[i].gauche, temp))
			return (domain->record + i);
	}
	
	return NULL;
}

void dns_parse(unsigned char *buffer, int size, namedconf_md_t *named, struct sockaddr_in *psor, int sockfd) {
	char name[256], len;
	dns_header_t *header;
	dns_question_t *question;
	records_md_t *record;
	domaine_md_t *dom = NULL;
	netid_md_t *nid = NULL;
	dns_rdata_t rdata;
	char *reply;
	int rc, newsize, ip;
	
	dump(buffer, size);
	
	header = (dns_header_t*) buffer;
	
	/* Delegate hack */
	__pkt      = buffer;
	__pkt_size = size;
	__sockfd   = sockfd;
	__psor     = psor;
	
	printf("[ ] DNS Parser: id     : %d\n", header->id);
	printf("[ ] DNS Parser: qu/res : %d (%s)\n", header->qr, __dns_qr(header->qr));
	printf("[ ] DNS Parser: opcode : %d (%s)\n", header->opcode, __dns_opcode(header->opcode));
	printf("[ ] DNS Parser: nbquest: %d\n", ntohs(header->q_count));
	
	len = tohost(buffer + sizeof(dns_header_t), buffer, name);
	question = (dns_question_t*)((char*) buffer + sizeof(dns_header_t) + len);
	
	
	printf("[ ] DNS Parser: request: class : %d (%s)\n", htons(question->qclass), __dns_class(htons(question->qclass)));
	printf("[ ] DNS Parser: request: type  : %d (%s)\n", htons(question->qtype), __dns_type(htons(question->qtype)));
	printf("[+] DNS Parser: request: string: %s\n", name);
	
	switch(htons(question->qtype)) {
		case A:
			dom = search_domaine(named->domaine, name);
			if(dom) {
				printf("[+] Domain found (%s). Searching record...\n", dom->nom);
				record = search_record(dom, htons(question->qtype), name);
				
				if(record == 1)
					return;
					
				if(record) {
					printf("[+] Answer: %s/%s/%s\n", record->gauche, __dns_type(record->type), record->droite);
					
				} else printf("[-] Record not found :(\n");
				
			} else printf("[-] Domaine not found :(\n");
		break;
		
		case PTR:
			nid = search_netid(named->netid, name);
			if(nid) {
				printf("[+] Netid found (%d.%d.%d.%d)\n", nid->o1, nid->o2, nid->o3, nid->o4);
				
				record = search_record_ptr(nid, name);
				if(record == 1)
					return;
					
				if(record) {
					printf("[+] Answer: %s/%s/%s\n", record->gauche, __dns_type(record->type), record->droite);
					
				} else printf("[-] Record not found :(\n");
			
			} else printf("[-] Domaine not found :(\n");
		break;
		
		default:
			record = NULL;
	}
	
	/* Working on answer */
	newsize = size - 1;
	reply = (char*) malloc(sizeof(char) * 512);
	
	/* Making answer */
	header->qr = RESPONSE;
	
	if(record) {
		header->ans_count = htons(1);
		
		/* Copy anwser type */
		rdata.rclass   = htons(IN);
		rdata.rtype    = htons(record->type);
		rdata.rttl     = htonl(42);	/* TTL fixed to 42 */
		
		if(record->type == PTR)
			rdata.data_len = htons(strlen(name));
			
		else rdata.data_len = htons(4);		/* IPv4 */
		
		/* Writing rdata */
		*(reply + newsize++) = 0xc0;
		*(reply + newsize++) = 0x0c;
		
		memcpy(reply + newsize, &rdata, sizeof(dns_rdata_t));		
		newsize += sizeof(dns_rdata_t);
		
		/* Writing answer */
		if(record->type == PTR) {
			/* Copy answer name */
			fromhost((unsigned char*) name, record->droite);
			
			memcpy(reply + newsize, name, strlen(name));
			newsize += strlen(name);
		
		} else {
			inet_aton(record->droite, (struct in_addr *) &ip);
			memcpy(reply + newsize, &ip, 4);
			newsize += 4;
		}
		
		
	} else header->ans_count = 0;
	
	/* Copy original question and new header */
	memcpy(reply, buffer, size - 1);
	
	if((rc = SendDatagram(sockfd, reply, newsize, psor)) == -1)
		perror("SendDatagram:");
		
	else printf("[+] Answer: sent: %d bytes\n", rc);
	
	free(reply);
}

