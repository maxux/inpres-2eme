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

	transac_t transaction;

	memset(&psoo,0,sizeof(struct sockaddr_in));
	memset(&psos,0,sizeof(struct sockaddr_in)); 
	memset(&psor,0,sizeof(struct sockaddr_in)); 

	printf("[+] Initializing Server (version %.1f - DANIEL Maxime)...\n", VERSION);
	if(argc != 5) {
		printf("ser ser port cli port\n") ;
		exit(1);
	}

	/* Opening files */
	if(!(fp = ticket_init())) {
		fprintf(stderr, "[-] Cannot open data file\n");
		exit(EXIT_FAILURE);
	}

	sockfd = CreateSockets(&psoo, &psos, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	if(sockfd == -1)
		perror("CreateSockets");
	else
		printf("CreateSockets: %d\n", sockfd);

	/* Waiting data */
	rc = ReceiveDatagram(sockfd, &transaction, sizeof(transaction), &psor);
	
	if(rc == -1)
		perror("ReceiveDatagram");
	else
		fprintf(stderr, "[+] Read: %d bytes. Action: %u\n", rc, transaction.action);
	
	transaction.numticket = ticket_reserver(fp, transaction.heure);

	/* Responding */
	rc = SendDatagram(sockfd, &transaction, sizeof(transaction), &psor);
	if(rc == -1)
		perror("SendDatagram:");
	else
		fprintf(stderr, "[+] Sent: %d\n", rc);
	
	ticket_lister(fp);
	
	return 0;
}
