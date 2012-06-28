#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "named.h"
#include "parser.h"
#include "../common/physlib/physlib.h"
#include "dns_parse.h"
#include "dns.h"

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

void dns_parse(unsigned char *buffer, int size, namedconf_md_t *named) {
	char name[256], len;
	dns_header_t *header;
	dns_question_t *question;
	
	domaine_md_t *dom = NULL;
	netid_md_t *nid = NULL;
	
	dump(buffer, size);
	
	header = (dns_header_t*) buffer;
	
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
			if(dom)
				printf("[+] DOMAIN <%s> FILE <%s>\n", dom->nom, dom->fichier);
				
			else printf("[-] DOMAIN: Not found :(\n");
		break;
		
		case PTR:
			nid = search_netid(named->netid, name);
			if(nid)
				printf("[+] NETID <%d.%d.%d.%d> FILE <%s>\n", nid->o1, nid->o2, nid->o3, nid->o4, nid->fichier);
			
			else printf("[-] NETID: Not found :(\n");
		break;
	}
}
