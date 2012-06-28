#ifndef __DNS_PARSE
	#define __DNS_PARSE

	void dns_parse(unsigned char *buffer, int size, namedconf_md_t *named, struct sockaddr_in *psor, int sockfd);
	
	char * __dns_opcode(dns_opcode_t opcode);
	char * __dns_qr(dns_query_t query);
	char * __dns_type(dns_type_t type);
	char * __dns_class(dns_class_t class);

#endif
