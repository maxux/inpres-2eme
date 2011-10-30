#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AccFichVar.h"
#include "interface-link.h"

int interface_consultation(FICHIERVAR *arg) {
	char buffer[32];
	int id = 0;
	
	printf(" -- Veuillez entrer un identifiant\n");
	
	while(1) {
		printf("Consultation > ");
		
		if(fgets(buffer, sizeof(buffer), stdin) == NULL)
			continue;
		
		id = atoi(buffer);
		
		if(id != 0 || (id == 0 && *buffer == '0'))
			break;
	}
	
	if(id == -1) {
		id = 0;
		
		while(FVConsultation(id, arg) != -1)
			id++;
		
	} else {
		if(FVConsultation(id, arg) == -1)
			fprintf(stderr, "[-] Core: ID Not Found\n");
	}
	
	return 0;
}

int interface_ajout(FICHIERVAR *arg) {
	char buffer[1024];
	int rc;
	
	printf("Ajout > ");
	
	while(fgets(buffer, sizeof(buffer), stdin) == NULL);
	
	rc = strlen(buffer);
	
	if(buffer[rc - 1] == '\n')
		buffer[rc - 1] = '\0';
		
	rc = FVAjoutFichier(buffer, arg);
	
	if (rc == -1) {
		perror("[-] Core: Insert failed");
		exit(1);
	}
	
	if(rc == 0)
		fprintf(stderr, "[-] Core: No space left\n");
	
	return 0;
}

int interface_affiche(FICHIERVAR *arg) {
	FVAffiche(arg);
	return 0;
}

int interface_suppression(FICHIERVAR *arg) {
	char buffer[32];
	int id = 0;
	
	printf(" -- Veuillez entrer un identifiant\n");
	
	while(1) {
		printf("Suppression > ");
		
		if(fgets(buffer, sizeof(buffer), stdin) == NULL)
			continue;
		
		id = atoi(buffer);
		
		if(id != 0 || (id == 0 && *buffer == '0'))
			break;
	}
	
	if(FVSuppression(id, arg) == -1)
		fprintf(stderr, "[-] Core: ID Not Found\n");
	
	return 0;
}
