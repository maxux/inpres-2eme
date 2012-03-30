#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barriere.h"
#include "../common/physlib/physlib.h"
#include "../common/transac.h"

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
	int rc, sockfd;
	char buffer[32];

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

	printf("BarriereEntree - DANIEL Maxime - 2227\n");

	while(1) {
		/* Creating transaction */
		printf("[ ] Heure du ticket: ");
		while(!fgets(buffer, sizeof(buffer), stdin));
		
		if(*(buffer) == '-' && *(buffer+1) == '1') {
			transac_list(fp);
			continue;
		}
		
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
		
		/* Writing data */
		fseek(fp, 0, SEEK_END);
		me.transac_id = tid++;
		me.numticket  = transaction.numticket;
		
		// printf("[+] Writing: Transaction: %ld, Ticket: %ld\n", me.transac_id, me.numticket);
		
		if(fwrite(&me, sizeof(me), 1, fp) != 1) {
			perror("fwrite");
			return -1;
		}
	}
	
	close(sockfd);
	
	return 0;
}
