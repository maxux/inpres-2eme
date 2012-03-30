#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sortie.h"
#include "../common/physlib/physlib.h"
#include "../common/transac.h"
#include "socket.h"

int actual_transac_id(FILE *fp) {
	int i = 0;
	transaction_t transaction;
	
	rewind(fp);
	while(fread(&transaction, sizeof(transaction), 1, fp) == 1)
		i++;
	
	return i;
}

void transac_list(FILE *fp) {
	transaction_t trans;
	
	rewind(fp);
	while(fread(&trans, sizeof(trans), 1, fp) == 1)
		printf("ID %ld: Ticket: %ld\n", trans.transac_id, trans.numticket);
}

int main(int argc, char *argv[]) {
	int sockfd;
	char buffer[32], ticket[32];

	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	transac_t transaction;
	transaction_t me;
	
	FILE *fp;
	char tfname[32];
	int tid;

	memset(&psoo, 0, sizeof(struct sockaddr_in));
	memset(&psos, 0, sizeof(struct sockaddr_in)); 
	memset(&psor, 0, sizeof(struct sockaddr_in)); 

	if (argc != 5) {
		printf("%s client-bind client-port server-address server-port\n", argv[0]);
		return 1;
	}
	
	sprintf(tfname, "client-%s.dat", argv[2]);
	fp = fopen(tfname, "r+");
	if(!fp) {
		printf("[-] Client data file doesn't exists. Creating...\n");
		fp = fopen(tfname, "w+");
		
		if(!fp) {
			perror("fopen");
			return 1;
			
		} else printf("[+] File created\n");
	}
	
	/* Enabling non-buffer on fp */
	if(setvbuf(fp, NULL, _IONBF, 0))
		perror("[-] setvbuf");
	
	/* Loading transaction id */
	tid = actual_transac_id(fp);
	
	printf("[+] Transaction ID: %d\n", tid);
	
	sockfd = CreateSockets(&psoo, &psos, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	if(sockfd == -1)
		perror("CreateSockets") ;
	else
		fprintf(stderr, "CreateSockets %d\n", sockfd);

	printf("SortieParking - DANIEL Maxime - 2227\n");

	while(1) {
		/* Creating transaction */
		printf("[ ] Numéro du ticket: ");
		while(!fgets(ticket, sizeof(ticket), stdin));
		
		printf("[ ] Heure du ticket: ");
		while(!fgets(buffer, sizeof(buffer), stdin));
		
		memset(&transaction, 0, sizeof(transaction));
		transaction.action     = EXIT;
		transaction.numticket  = atoi(ticket);
		transaction.heure      = atoi(buffer);
		transaction.transac_id = tid;

		if(transaction_transmit(sockfd, &transaction, sizeof(transac_t), &psos, &psor) == 0) {
			/* Checking response */
			if(transaction.action == EXIT) {
				printf("\n");
				printf("+------------------------------------+\n");
				printf("| Merci de votre visite, à bientôt ! |\n");
				printf("+------------------------------------+\n");
				
			} else {
				printf("[-] Ticket invalide\n");
				continue;
			}
			
			/* Writing data */
			fseek(fp, 0, SEEK_END);
			me.transac_id = tid++;
			me.numticket  = transaction.numticket;
			
			printf("[+] Writing: Transaction: %ld, Ticket: %ld\n", me.transac_id, me.numticket);
			
			if(fwrite(&me, sizeof(me), 1, fp) != 1) {
				perror("fwrite");
				return -1;
			}
			
			transac_list(fp);
			
		} else fprintf(stderr, "[-] Cannot contact server\n");
	}
	
	close(sockfd);
	
	return 0;
}
