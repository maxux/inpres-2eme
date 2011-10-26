#include <stdio.h>
#include <stdlib.h>

#include "AccFichVar.h"
#include "Applic.h"

FICHIERVAR F;

int main() {
	int rc;
	F.Taille = DATA_SIZE;
	
	int testc = 4, i;
	char *testv[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
			"bbb",
			"cccccccccccccccccccc",
			"ddddddddddddddddddddddd"};
	
	printf("Debut de l'Applic\n");

	if(FVOuvertureFichier("Data.dat", &F)) {
		perror("[-] Err. d'ouverture du fichier");
		exit(1);
	}

	debug("[ ] File Descriptiors: data: %d   index: %d\n", F.hdF, F.hdIndex);
	FVAffiche(&F);

	for(i = 0; i < testc; i++) {
		rc = FVAjoutFichier(testv[i], &F);
		
		if (rc == -1) {
			perror("[-] Insert failed");
			exit(1);
		}
		
		if(rc == 0)
			debug("[-] No space left\n");

		FVAffiche(&F);
	}
	
	if(FVConsultation(1, &F) == -1)
		fprintf(stderr, "[-] ID not found\n");
	
	if(FVSuppression(1, &F) == -1)
		fprintf(stderr, "[-] Cannot remove this ID\n");
	
	FVAffiche(&F);

	if(FVFermetureFichier(&F)) {
		perror("[-] Err. de fermeture du fichier");
		exit(1);
	}
	
	exit(0);
}
