#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "named.h"
#include "parser.h"
#include "../common/physlib/physlib.h"
#include "dns_parse.h"

int main(void) {
	namedconf_md_t *named;
	int sockfd, rc;
	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	char buffer[1024];
	
	if(!(named = lecture_namedconf_md(NULL)))
		return 1;
	
	debug_domaine(named->domaine);
	debug_netid(named->netid);
	
	/* Init Socket */
	printf("[+] Socket: creating...\n");
	if((sockfd = CreateSockets(&psoo, NULL, BIND_ADDRESS, BIND_PORT, NULL, 0)) < 0) {
		fprintf(stderr, "[-] Socket: error\n");
		exit(EXIT_FAILURE);
	}
	
	/* Listening */
	while(1) {
		rc = ReceiveDatagram(sockfd, buffer, sizeof(buffer), &psor);
		
		if(rc == -1) {
			perror("ReceiveDatagram");
			continue;
			
		} else fprintf(stderr, "[+] New data ! Size: %d bytes\n", rc);
		
		dns_parse((unsigned char*) buffer, rc, named);
	}
	
	return 0;
}
