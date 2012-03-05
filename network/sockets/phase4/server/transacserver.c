#include <stdio.h>
#include <stdlib.h>
#include "../common/transac.h"
#include "transacserver.h"

transac_t ticket_read_header(FILE *fp) {
	transac_t transaction;
	int before;
	
	/* Saving */
	before = ftell(fp);
	
	/* Rewind */
	rewind(fp);
	
	/* Reading */
	if(fread(&transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fread");
		transaction.action = ERROR;
		return transaction;
	}
	
	if(transaction.action != HEADER) {
		fprintf(stderr, "[-] File error\n");
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, before, SEEK_SET);
	
	return transaction;
}

void ticket_write_header(FILE *fp, transac_t *transaction) {
	int before;
	
	/* Saving */
	before = ftell(fp);
	
	/* Rewind */
	rewind(fp);
	
	/* Writing */
	if(fwrite(transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fwrite");
		return;
	}
	
	fseek(fp, before, SEEK_SET);
}

transac_t ticket_reserver(FILE *fp, transac_t parent) {
	transac_t transaction;
	
	transaction = ticket_read_header(fp);
	if(transaction.placelibre < 1) {
		fprintf(stderr, "[-] No more place\n");
		
		transaction.numticket = -1;
		return transaction;
	}
	
	transaction.placelibre--;
	transaction.numticket++;
	
	ticket_write_header(fp, &transaction);
	
	transaction.action     = RESERVATION;	
	transaction.heure      = parent.heure;
	transaction.ip         = parent.ip;
	transaction.port       = parent.port;
	transaction.transac_id = parent.transac_id;
	
	fseek(fp, 0, SEEK_END);
	if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fwrite");
		
		transaction.numticket = -1;
		return transaction;
	}
	
	return transaction;
}

void ticket_payer(FILE *fp) {
	transac_t header, transaction;
	char buffer[32];
	int tid;
	
	printf("Ticket: ");
	while(!fgets(buffer, sizeof(buffer), stdin));
	
	tid = atoi(buffer);
	
	fseek(fp, - sizeof(transac_t), SEEK_END);
	
	while(fread(&transaction, sizeof(transac_t), 1, fp) == 1) {
		if(transaction.numticket == tid && transaction.action == BUY)
			break;
		
		if(transaction.numticket == tid && transaction.action == RESERVATION)
			break;
		
		if(ftell(fp) == sizeof(transac_t))
			break;
		
		fseek(fp, - (sizeof(transac_t) * 2), SEEK_CUR);
	}
	
	if(transaction.action == BUY) {
		printf("[-] Ticket déjà payé\n");
		return;
	}
	
	if(transaction.numticket != tid) {
		printf("[-] Ticket non trouvé\n");
		return;
	}
	
	header = ticket_read_header(fp);
	
	printf("Heure: ");
	while(!fgets(buffer, sizeof(buffer), stdin));
	
	transaction.placelibre = header.placelibre;
	transaction.heure      = atoi(buffer);
	// transaction.transac_id = ++header.transac_id;
	transaction.action     = BUY;
	
	fseek(fp, 0, SEEK_END);
	if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fwrite");
		return;
	}
}

void ticket_sortir(FILE *fp) {
	transac_t header, transaction;
	char buffer[32];
	int heure, tid;
	
	printf("Ticket: ");
	while(!fgets(buffer, sizeof(buffer), stdin));
	
	tid = atoi(buffer);
	
	fseek(fp, - sizeof(transac_t), SEEK_END);
	
	while(fread(&transaction, sizeof(transac_t), 1, fp) == 1) {
		if(transaction.numticket == tid && transaction.action == BUY)
			break;
		
		if(transaction.numticket == tid && transaction.action == EXIT)
			break;
		
		if(ftell(fp) == sizeof(transac_t))
			break;
		
		fseek(fp, - (sizeof(transac_t) * 2), SEEK_CUR);
	}
	
	if(transaction.numticket != tid) {
		printf("[-] Ticket non trouvé\n");
		return;
	}
	
	if(transaction.action == EXIT) {
		printf("[-] Ticket déjà sorti\n");
		return;
	}
	
	header = ticket_read_header(fp);
	
	printf("Heure: ");
	while(!fgets(buffer, sizeof(buffer), stdin));
	
	heure = atoi(buffer);
	
	if(transaction.heure != heure) {
		printf("[-] Ticket périmé\n");
		return;
	}
	
	transaction.placelibre = header.placelibre++;
	// transaction.transac_id = ++header.transac_id;
	transaction.heure      = heure;
	transaction.action     = EXIT;
	
	fseek(fp, 0, SEEK_END);
	if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fwrite");
		return;
	}
	
	ticket_write_header(fp, &header);
}

void ticket_lister(FILE *fp) {
	transac_t transaction;
	
	rewind(fp);
	
	while(fread(&transaction, sizeof(transac_t), 1, fp))
		printf("[ ] Type: 0x%x, Heure: %02ld, PlaceLibre: %ld, NumTicket: %ld, Transaction: %ld, IP/Port: %ld:%ld\n",
			transaction.action, transaction.heure, transaction.placelibre, transaction.numticket, transaction.transac_id, transaction.ip, transaction.port);
	
	rewind(fp);
}

FILE * ticket_init() {
	FILE *fp;
	transac_t transaction;
	
	printf("[+] Opening file\n");
	
	fp = fopen(TR_FILENAME, "r+");
	if(fp) {
		printf("[+] Checking file validity\n");
		
		if(fread(&transaction, sizeof(transac_t), 1, fp) != 1) {
			perror("[-] fread");
			return NULL;
		}
		
		if(transaction.action != HEADER) {
			fprintf(stderr, "[-] Invalid header\n");
			fclose(fp);
			
			return NULL;
		}
			
	} else {
		printf("[+] Cannot open file, creating file\n");
		fp = fopen(TR_FILENAME, "w+");
		
		if(!fp) {
			perror("fopen");
			return NULL;
		}
		
		transaction.placelibre	= PLACE_MAX;
		transaction.numticket	= 0;
		transaction.transac_id  = 0;
		transaction.heure	= 0;
		transaction.ip		= 0;
		transaction.port	= 0;
		transaction.action	= HEADER;
		
		if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1)
			perror("[-] fwrite");
	}
	
	if(setvbuf(fp, NULL, _IONBF, 0))
		perror("[-] setvbuf");
	
	return fp;
}

int transac_check_exists(FILE *fp, transac_t *trans) {
	transac_t temp;
	
	fseek(fp, sizeof(transac_t), SEEK_SET);
	while(fread(&temp, sizeof(temp), 1, fp) == 1) {		
		if(temp.port == trans->port && temp.ip == trans->ip && temp.transac_id == trans->transac_id)
			return 1;
	}
	
	return 0;
}
