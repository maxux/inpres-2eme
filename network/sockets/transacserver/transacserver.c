#include <stdio.h>
#include <stdlib.h>
#include "transacserver.h"

#define TR_FILENAME	"transac.dat"
#define PLACE_MAX	99

transac_t read_header(FILE *fp) {
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

void write_header(FILE *fp, transac_t *transaction) {
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

void reserver_ticket(FILE *fp) {
	transac_t transaction;
	char buffer[32];
	
	transaction = read_header(fp);
	if(transaction.placelibre < 1) {
		fprintf(stderr, "No more place\n");
		return;
	}
	
	transaction.placelibre--;
	transaction.numticket++;
	
	write_header(fp, &transaction);
	
	transaction.action = RESERVATION;
	
	/* FGETS */
	transaction.heure  = atoi(buffer);
	
	fseek(fp, 0, SEEK_END);
	if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1) {
		perror("[-] fwrite");
		return;
	}
}

void payer_ticket(FILE *fp) {
	
}

void sortir_parking(FILE *fp) {
	
}

void lister_contenu(FILE *fp) {
	
}

FILE * init_transac() {
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
		transaction.heure	= 0;
		transaction.action	= HEADER;
		
		if(fwrite(&transaction, sizeof(transac_t), 1, fp) != 1)
			perror("[-] fwrite");
	}
	
	return fp;
}

int main(void) {
	FILE *fp;
	int menuid;
	char buffer[32];
	
	/* Init */
	fp	= NULL;
	menuid	= -1;
	
	while(menuid != 6) {
		printf("--- DANIEL Maxime - 2227\n");
			
		printf(" 1) Créer le fichier transaction\n");
		printf(" 2) Réserver un ticket\n");
		printf(" 3) Payer le ticket\n");
		printf(" 4) Sortir du parking\n");
		printf(" 5) Lister le contenu du fichier\n");
		printf(" 6) Quitter\n");

		printf("# ");
		while(fgets(buffer, sizeof(buffer), stdin) == NULL);
		menuid = atoi(buffer);
		
		switch(menuid) {
			case 1:
				if(fp)
					fclose(fp);
				
				if(!(fp = init_transac())) {
					fprintf(stderr, "[-] Cannot open data file\n");
					exit(EXIT_FAILURE);
				}
			break;
			
			case 2:
				if(!fp) {
					printf("[-] No file selected\n");
					break;
				}
				
				reserver_ticket(fp);
			break;
			
			case 3:
				if(!fp) {
					printf("[-] No file selected\n");
					break;
				}
				
				payer_ticket(fp);
			break;
			
			case 4:
				if(!fp) {
					printf("[-] No file selected\n");
					break;
				}
				
				sortir_parking(fp);
			break;
			
			case 5:
				if(!fp) {
					printf("[-] No file selected\n");
					break;
				}
				
				lister_contenu(fp);
			break;
			
			case 6:
				printf("Closing...\n");
			break;
			
			default:
				printf("Commande inconnue\n");
		}
	}

	fclose(fp);
	
	return 0;
}
