#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "AccFichVar.h"
#include "Applic.h"
#include "interface.h"
#include "interface-link.h"

FICHIERVAR F;

void sig_handler(int signum) {
	switch(signum) {
		case SIGINT:
		case SIGTERM:
			printf("\n");
			if(FVFermetureFichier(&F)) {
				perror("[-] Core: Err. de fermeture du fichier");
				exit(EXIT_FAILURE);
			}
			
			exit(EXIT_SUCCESS);
			
		break;
	}
}

int main() {
	menu_t *menu;
	F.Taille = DATA_SIZE;
	
	debug("[+] Core: Init Application\n");
	
	/* Intercept SIG to Commit Index */
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	/* Opening files */
	if(FVOuvertureFichier("Data.dat", &F)) {
		perror("[-] Core: Err. d'ouverture du fichier");
		exit(1);
	}

	debug("[ ] Core: File Descriptiors: data: %d   index: %d\n", F.hdF, F.hdIndex);
	
	/* Menu Code */
	menu = menu_create("Application Unix - Menu Principal", 9);
	menu_append(menu, "Faire une consultation", 1, interface_consultation, &F);
	menu_append(menu, "Ajouter une entrée", 2, interface_ajout, &F);
	menu_append(menu, "Supprimer une entrée", 3, interface_suppression, &F);
	menu_append(menu, "Afficher la table de debuggage", 4, interface_affiche, &F);
	menu_append(menu, "Quitter l'application", 9, NULL, NULL);
	
	/* Menu Process */
	while(menu_process(menu));
	menu_free(menu);
	
	/* Closing */
	if(FVFermetureFichier(&F)) {
		perror("[-] Core: Err. de fermeture du fichier");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
