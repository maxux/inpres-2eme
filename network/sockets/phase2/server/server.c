#include <stdio.h>
#include <stdlib.h>
#include "../common/transac.h"
#include "../common/physlib/physlib.h"
#include "server.h"
#include "transacserver.h"

int main(int argc,char *argv[]) {
	int rc, sockfd;
	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	FILE *fp;
	
	char *bind_address;
	int port;

	transac_t transaction;

	memset(&psoo,0,sizeof(struct sockaddr_in));
	memset(&psos,0,sizeof(struct sockaddr_in)); 
	memset(&psor,0,sizeof(struct sockaddr_in)); 

	printf("[+] Initializing Server (version %.1f - DANIEL Maxime)...\n", VERSION);
	if(argc == 1) {
		bind_address = DEFAULT_ADDRESS;
		port = DEFAULT_PORT;
		
	} else if(argc == 3) {
		bind_address = argv[1];
		port = atoi(argv[2]);
		
	} else {
		printf("%s [[address-bind] [port]]\n", argv[0]) ;
		exit(1);
	}

	/* Opening files */
	if(!(fp = ticket_init())) {
		fprintf(stderr, "[-] Cannot open data file\n");
		exit(EXIT_FAILURE);
	}

	sockfd = CreateSockets(&psoo, NULL, bind_address, port, NULL, 0);
	if(sockfd == -1)
		perror("CreateSockets");
	else
		printf("CreateSockets: %d\n", sockfd);

	while(1) {
		/* Waiting data */
		rc = ReceiveDatagram(sockfd, &transaction, sizeof(transaction), &psor);
		
		if(rc == -1)
			perror("ReceiveDatagram");
		else
			fprintf(stderr, "[+] Read: %d bytes. Action: %u\n", rc, transaction.action);
		
		switch(transaction.action) {
			case RESERVATION:
				transaction.numticket = ticket_reserver(fp, transaction.heure);

				/* Responding */
				rc = SendDatagram(sockfd, &transaction, sizeof(transaction), &psor);
				if(rc == -1)
					perror("SendDatagram:");
				else
					fprintf(stderr, "[+] Sent: %d\n", rc);
			break;
			
			default:
				fprintf(stderr, "[-] Unknown protocol request\n");
		}
		
		printf("[x] DEBUG Table:\n");
		ticket_lister(fp);
	}
	
	return 0;
}
