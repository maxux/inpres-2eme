#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "AccFichVar.h"
#include "Applic.h"
#include "interface.h"

int PremierLibre;

int FVOuvertureFichier(const char *NomFichier, FICHIERVAR *Fich) {
	ENREG E;
	int i;
	
	// memcpy(E.Donnee, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20);
	memset(E.Donnee, '\0', Fich->Taille);
	
	debug("[>] Calling: FVOuvertureFich()\n");
	debug("[+] FVOuvertureFichier: Checking files presence...\n");
	
	if((Fich->hdF = open(NomFichier, O_RDWR)) == -1) {
		debug("[ ] FVOuvertureFichier: Creating %s...\n", NomFichier);
		if((Fich->hdF = open(NomFichier, O_RDWR | O_CREAT, 0644)) == -1)
			return -1;

		debug("[ ] FVOuvertureFichier: Creating Index...\n");
		if((Fich->hdIndex = open("Index", O_RDWR | O_CREAT, 0644)) == -1)
			return -1;

		debug("[ ] FVOuvertureFichier: Building data file...\n");
		for(i = 1; i <= Fich->Taille; i++) {
			if(i == 20)
				E.Suivant = 0;
			else				
				E.Suivant = i;
			
			if(write(Fich->hdF, &E, sizeof(E)) != sizeof(E))
				return -1;
		}
		
		/* Allocating: Index -- [record id][record size] */
		debug("[ ] FVOuvertureFichier: Building Index...\n");
		if((Fich->Index = (int*) calloc(Fich->Taille * 2, sizeof(int))) == NULL)
			return -1;
		
		debug("[ ] FVOuvertureFichier: Writing Index file...\n");
		if(write(Fich->hdIndex, Fich->Index, Fich->Taille * sizeof(index_element_t)) != (signed)(Fich->Taille * sizeof(index_element_t)))
			return -1;

		debug("[+] FVOuvertureFichier: Writing successful\n");
		PremierLibre = 1;
		
		debug("[ ] FVOuvertureFichier: First free: %d\n", PremierLibre);
		
		return 0;
	}
	
	debug("[ ] FVOuvertureFichier: Reading data file...\n");
	if(read(Fich->hdF, &E, Fich->Taille + sizeof(int)) != (signed)(Fich->Taille + sizeof(int)))
		return -1;

	PremierLibre = E.Suivant;
	debug("[ ] FVOuvertureFichier: First free: %d\n", PremierLibre);
	
	debug("[ ] FVOuvertureFichier: Allocating index...\n");
	if((Fich->Index = (int*)malloc(Fich->Taille * sizeof(index_element_t))) == NULL)
		return -1;

	debug("[ ] FVOuvertureFichier: Opening Index...\n");
	if((Fich->hdIndex = open("Index", O_RDWR)) == -1)
		return -1;
	
	debug("[ ] FVOuvertureFichier: Loading index...\n");
	if(read(Fich->hdIndex, Fich->Index, Fich->Taille * sizeof(index_element_t)) != (signed)(Fich->Taille * sizeof(index_element_t)))
		return -1;
	
	debug("[+] FVOuvertureFichier: Reading files successfull\n");
	return 0;
}

int FVFermetureFichier(FICHIERVAR *Fich) {
	debug("[>] Calling: FVFermetureFichier\n");
	debug("[>] FVFermetureFichier: Closing files\n");

	/* Closing Data File */
	lseek(Fich->hdF, sizeof(ENREG) - sizeof(int), SEEK_SET);
	if(write(Fich->hdF, &PremierLibre, sizeof(PremierLibre)) != sizeof(PremierLibre))
		return -1;
		
	if(close(Fich->hdF))
		return -1;

	debug("[ ] FVFermetureFichier: Commit Index...\n");
	/* Commit Index File */
	lseek(Fich->hdIndex, 0, SEEK_SET);
	if(write(Fich->hdIndex, Fich->Index, sizeof(int) * Fich->Taille * 2) != (signed) sizeof(int) * Fich->Taille * 2)
		return -1;
		
	/* Closing Index */
	if(close(Fich->hdIndex))
		return -1;
	
	debug("[+] FVFermetureFichier: Files closed...\n");
		
	return 0;
}

int FVAjoutFichier(const char* Info, FICHIERVAR *Fich) {
	ENREG E;
	int Pos;
	const char *pInter = Info;
	int i;
	
	debug("[>] Calling: FVAjoutFich()\n");
	debug("[ ] FVAjoutFichier: Request: <%s> (Len: %lu)\n", Info, (unsigned long) strlen(Info));
	
	/* Checking space */
	if(PremierLibre == 0)
		return 0;
	
	debug("[ ] FVAjoutFichier: First free: %d\n", PremierLibre);
	Pos = PremierLibre;
	
	i = 0;
	while(*(Fich->Index + i))
		i += 2;
	
	/* Writing current id, incrementing i to point to segment count */
	*(Fich->Index + i) = PremierLibre;
	
	debug("[ ] FVAjoutFichier: New Item ID: %d\n", PremierLibre);
	debug("[ ] FVAjoutFichier: Index Offset: %d\n", i);
	
	/* Pointing to Length */
	i++;
	
	/* Spliting write if needed */
	while(strlen(pInter) > (unsigned) Fich->Taille) {
		debug("[ ] FVAjoutFichier: Inserting Segment...\n");
		
		if(lseek(Fich->hdF, Pos * sizeof(E), SEEK_SET) == -1)
			return -1;
			
		if(read(Fich->hdF, &E, sizeof(E)) != sizeof(E))
			return -1;
			
		memcpy(E.Donnee, pInter, Fich->Taille);
		
		if(lseek(Fich->hdF, Pos * sizeof(E), SEEK_SET) == -1)
			return -1;
			
		if(write(Fich->hdF, &E, sizeof(E)) != sizeof(E))
			return -1;
			
		Pos = E.Suivant;
		pInter += Fich->Taille;
		
		(*(Fich->Index + i))++;
		
		/* No more space after this segment. Stopping. */
		if(E.Suivant == 0) {
			debug("[-] FVAjoutFichier: No more space after this\n");
			PremierLibre = 0;
			return 0;
		}
	}
	
	debug("[+] FVAjoutFichier: Writing Last Segment\n");
	
	/* Writing last element */
	if(lseek(Fich->hdF, Pos * (Fich->Taille + sizeof(int)), SEEK_SET) == -1)
		return -1;
		
	if(read(Fich->hdF, &E, (Fich->Taille + sizeof(int))) != (signed) (Fich->Taille + sizeof(int)))
		return -1;
	
	/* Saving current next free element */
	PremierLibre = E.Suivant;
	
	/* Marking this element as the last */
	E.Suivant    = 0;
	
	/* Copy data */
	memcpy(E.Donnee, pInter, Fich->Taille);
	
	/* Writing data */
	if(lseek(Fich->hdF,Pos * (Fich->Taille + sizeof(int)), SEEK_SET) == -1)
		return -1;
		
	if(write(Fich->hdF, &E, (Fich->Taille + sizeof(int))) != (signed) (Fich->Taille + sizeof(int)))
		return -1;
	
	debug("[ ] FVAjoutFichier: Updating Index\n");
	/* Increment Index Segment Count */
	(*(Fich->Index + i))++;
	
	/* Commit First Free */
	if(lseek(Fich->hdF, 0, SEEK_SET) == -1)
		return -1;
	
	debug("[+] FVAjoutFichier: Insert Request Sucessfull\n");
	
	return 1;
}

void FVAffiche(FICHIERVAR *Fich) {
	ENREG E;
	int i = 1;
	char Buff[21];
	
	Buff[20] = '\0';
	
	lseek(Fich->hdF, sizeof(E), SEEK_SET);
	
	printf("\n[XX] -- %-20s -- [Next: %02d]\n", "First Free Element", PremierLibre);
	
	while(read(Fich->hdF, &E, sizeof(E)) > 0) {
		memcpy(Buff, E.Donnee, Fich->Taille);
		printf("[%02d] -- %-20s -- [Next: %02d]\n", i, Buff, E.Suivant);
		
		i++;
	}

	i = 0;
	while(i < Fich->Taille - 1) {
		printf("[%02d %02d] ", *(int *)(Fich->Index + i), *(int *)(Fich->Index + i + 1));
		i += 2;
	}
	
	printf("\n");
	
	while(i < (Fich->Taille * 2) - 1) {
		printf("[%02d %02d] ", *(int *)(Fich->Index + i), *(int *)(Fich->Index + i + 1));
		i += 2;
	}
	
	printf("\n\n");
	fflush(stdout);
}

int FVConsultation(int id, FICHIERVAR *Fich) {
	int i;
	char *buffer = NULL;
	index_element_t element;
	ENREG temp;
	
	debug("[>] Calling: FVConsultation()\n");
	debug("[ ] FVConsultation: Request ID: %d\n", id);
	
	if(id > Fich->Taille || id < 0)
		return -1;
	
	/* Affecting Index Structure Element. */
	element.id     = *(Fich->Index + (id * 2));
	element.length = *(Fich->Index + (id * 2) + 1);
		
	/* Checking if not empty */
	if(element.id == 0)
		return -1;
	
	debug("[ ] FVConsultation: Reading ID: %d\n", element.id);
	debug("[ ] FVConsultation: Record Segment Count: %d\n", element.length);
	
	/* Setting next id */
	temp.Suivant = element.id;
	
	buffer = (char*) malloc(sizeof(char) * Fich->Taille * element.length + 1);
	
	/* Reading data */
	for(i = 0; i < element.length; i++) {
		lseek(Fich->hdF, temp.Suivant * sizeof(temp), SEEK_SET);
		
		if(read(Fich->hdF, &temp, sizeof(temp)) != sizeof(temp))
			return -1;
		
		strncat(buffer, temp.Donnee, Fich->Taille);
	}
	
	debug("[+] FVConsultation: ID: %d - Buffer Length: %d\n", id, strlen(buffer));
	
	printf("%s\n", buffer);
	
	if(buffer != NULL)
		free(buffer);
	
	return 0;
}

int FVSuppression(int id, FICHIERVAR *Fich) {
	ENREG remove_item;
	index_element_t element;
	int newPremierLibre, i, *indexPointer;
	
	debug("[>] Calling: FVSuppression()\n");
	debug("[ ] FVSuppression: Request ID: %d\n", id);
	
	if(id > Fich->Taille || id < 0)
		return -1;
	
	/* Affecting Index Structure Element. */
	element.id     = *(Fich->Index + (id * 2));
	element.length = *(Fich->Index + (id * 2) + 1);
	
	debug("[ ] FVSuppression: Segment ID: %d\n", element.id);
	debug("[ ] FVSuppression: Segment count: %d\n", element.length);
		
	/* Checking if not empty */
	if(element.id == 0)
		return -1;
	
	debug("[ ] FVSuppression: Removing ID: %d\n", id);	
	debug("[ ] FVSuppression: Current First Free ID: %d\n", PremierLibre);
	
	/* Setting up "First free element" to this Segment */
	debug("[ ] FVSuppression: New First Free ID: %d\n", element.id);
	newPremierLibre = element.id;

	/* Setting up last data next free id */
	lseek(Fich->hdF, sizeof(remove_item) * element.id, SEEK_SET);
	
	for(i = 0; i < element.length; i++) {
		if(read(Fich->hdF, &remove_item, sizeof(remove_item)) != sizeof(remove_item))
			return -1;
			
		debug("[+] FVSuppression: Removing Segment: %d (Next ID: %d)\n", i, remove_item.Suivant);
		
		if(remove_item.Suivant != 0)
			lseek(Fich->hdF, sizeof(remove_item) * remove_item.Suivant, SEEK_SET);
	}
	
	/* Now: remove_item.Suivant is the last Data Segment */
	lseek(Fich->hdF, -sizeof(remove_item), SEEK_CUR);
	remove_item.Suivant = PremierLibre;
	
	/* Writing new Free Segment */
	if(write(Fich->hdF, &remove_item, sizeof(remove_item)) != sizeof(remove_item))
		return -1;
		
	PremierLibre = newPremierLibre;
	
	debug("[+] FVSuppression: Now, First Free ID: %d\n", PremierLibre);
	
	/* Updating Index */
	debug("[ ] FVSuppression: Updating Index\n");
	indexPointer = Fich->Index + (id * 2);
	
	while(*indexPointer != 0) {
		*indexPointer       = *(indexPointer + 2);
		*(indexPointer + 1) = *(indexPointer + 3);
		
		indexPointer += 2;
	}
	
	debug("[+] FVSuppression: Remove Segments Sucessfull\n");
	
	return 0;
}
