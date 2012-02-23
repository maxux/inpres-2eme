#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barriere.h"
#include "../common/physlib/physlib.h"
#include "../common/transac.h"

int main(int argc, char *argv[]) {
	int rc, sockfd;
	char buffer[32];

	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	transac_t transaction;

	memset(&psoo, 0, sizeof(struct sockaddr_in));
	memset(&psos, 0, sizeof(struct sockaddr_in)); 
	memset(&psor, 0, sizeof(struct sockaddr_in)); 

	if (argc != 5) {
		printf("cli client portc serveur ports\n") ;
		exit(1) ;
	}
	
	sockfd = CreateSockets(&psoo, &psos, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	if(sockfd == -1)
		perror("CreateSockets:") ;
	else
		fprintf(stderr, "CreateSockets %d\n", sockfd);

	printf("BarriereEntree - DANIEL Maxime - 2227\n");

	/* Creating transaction */
	printf("[ ] Heure du ticket: ");
	while(!fgets(buffer, sizeof(buffer), stdin));
	
	memset(&transaction, 0, sizeof(transaction));
	transaction.action = RESERVATION;
	transaction.heure  = atoi(buffer);

	/* Sending data */
	rc = SendDatagram(sockfd, &transaction, sizeof(transaction), &psos) ;

	if(rc == -1)
		perror("SendDatagram");
	else
		fprintf(stderr, "[+] Sending: %d bytes\n", rc);

	/* Clearing transaction */
	memset(&transaction, 0, sizeof(transaction));

	/* Waiting response */
	rc = ReceiveDatagram(sockfd, &transaction, sizeof(transaction), &psor) ;
	if(rc == -1)
		perror("ReceiveDatagram") ;
	else
		fprintf(stderr, "[+] Read: %d\n", rc) ;

	/* Checking response */
	if(transaction.action == RESERVATION) {
		printf("\n");
		printf("+----------------------+\n");
		printf("| Numéro du ticket: %02ld |\n", transaction.numticket);
		printf("+----------------------+\n");
		
	} else printf("[-] Data invalid\n");

	close(sockfd);
	
	return 0;
}
