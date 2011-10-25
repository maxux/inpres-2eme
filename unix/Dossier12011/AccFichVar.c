#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "AccFichVar.h"
#include "Applic.h"

int PremierLibre;

int FVOuvertureFichier(const char *NomFichier, FICHIERVAR *Fich) {
	ENREG E;
	int i;
	
	// memcpy(E.Donnee, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20);
	memset(E.Donnee, '\0', Fich->Taille);
	
	debug("[>] Entree dans FVOuvertureFich()\n");
	debug("[+] Checking files presence...\n");
	
	if((Fich->hdF = open(NomFichier, O_RDWR)) == -1) {
		debug("[ ] Creating %s...\n", NomFichier);
		if((Fich->hdF = open(NomFichier, O_RDWR | O_CREAT, 0644)) == -1)
			return -1;

		debug("[ ] Creating Index...\n");
		if((Fich->hdIndex = open("Index", O_RDWR | O_CREAT, 0644)) == -1)
			return -1;

		debug("[ ] Building data file...\n");
		for(i = 1; i <= Fich->Taille; i++) {
			if(i == 20)
				E.Suivant = 0;
			else				
				E.Suivant = i;
			
			if(write(Fich->hdF, &E, sizeof(E)) != sizeof(E))
				return -1;
		}
		
		/* Allocating: Index -- [record id][record size] */
		debug("[ ] Building Index...\n");
		if((Fich->Index = (int*) calloc(Fich->Taille * 2, sizeof(int))) == NULL)
			return -1;
		
		debug("[ ] Writing Index file...\n");
		if(write(Fich->hdIndex, Fich->Index, Fich->Taille * sizeof(int) * 2) != (signed)(Fich->Taille * sizeof(int) * 2))
			return -1;

		debug("[+] Writing successful\n");
		PremierLibre = 1;
		
		debug("[ ] First free: %d\n", PremierLibre);
		
		return 0;
	}
	
	debug("[ ] Reading data file...\n");
	if(read(Fich->hdF, &E, Fich->Taille + sizeof(int)) != (signed)(Fich->Taille + sizeof(int)))
		return -1;

	PremierLibre = E.Suivant;
	debug("[ ] First free: %d\n", PremierLibre);
	
	debug("[ ] Allocating index...\n");
	if((Fich->Index = (int*)malloc(Fich->Taille * sizeof(int) * 2)) == NULL)
		return -1;

	debug("[ ] Opening Index...\n");
	if((Fich->hdIndex = open("Index", O_RDWR)) == -1)
		return -1;
	
	debug("[ ] Loading index...\n");
	if(read(Fich->hdIndex, Fich->Index, Fich->Taille * sizeof(int)) != (signed)(Fich->Taille * sizeof(int)))
		return -1;
	
	debug("[+] Reading files successfull\n");
	return 0;
}

int FVFermetureFichier(FICHIERVAR *Fich) {
	debug("[>] Closing files\n");

	/* Closing Data File */
	if(close(Fich->hdF))
		return -1;

	debug("[ ] Commit Index...\n");
	/* Commit Index File */
	lseek(Fich->hdIndex, 0, SEEK_SET);
	if(write(Fich->hdIndex, Fich->Index, sizeof(int) * Fich->Taille * 2) != (signed) sizeof(int) * Fich->Taille * 2)
		return -1;
		
	/* Closing Index */
	if(close(Fich->hdIndex))
		return -1;
	
	debug("[+] Files closed...\n");
		
	return 0;
}

int FVAjoutFichier(const char* Info, FICHIERVAR *Fich) {
	ENREG E;
	int Pos;
	const char *pInter = Info;
	int i;
	
	debug("[>] Entree dans FVAjoutFich()\n");
	debug("(##) -- %s -- < Len: %ld\n", Info, strlen(Info));
	
	/* Checking space */
	if(PremierLibre == 0)
		return 0;
	
	debug("[ ] First free: %d\n", PremierLibre);
	Pos = PremierLibre;
	
	i = 0;
	while(*(Fich->Index + i))
		i += 2;
	
	/* Writing current id, incrementing i to point to segment count */
	*(Fich->Index + i++) = PremierLibre;
	
	/* Spliting write if needed */
	while(strlen(pInter) > (unsigned) Fich->Taille) {
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
			UpdateFirstElement(0, Fich);
			return 0;
		}
	}
	
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
	
	/* Increment Index Segment Count */
	(*(Fich->Index + i))++;
	
	/* Commit First Free */
	if(lseek(Fich->hdF, 0, SEEK_SET) == -1)
		return -1;
	
	if(UpdateFirstElement(PremierLibre, Fich) == -1)
		return -1;
	
	return 1;
}

int UpdateFirstElement(int id, FICHIERVAR *Fich) {
	/* Skipping data segment, writing directly new first free id */
	lseek(Fich->hdF, sizeof(ENREG) - sizeof(int), SEEK_SET);
	
	if(write(Fich->hdF, &id, sizeof(int)) != sizeof(int))
		return -1;
	
	return 0;
}

void FVAffiche(FICHIERVAR *Fich) {
	ENREG E;
	int i = 0;
	char Buff[21];
	
	Buff[20] = '\0';
	
	lseek(Fich->hdF, 0, SEEK_SET);
	
	printf("\n");
	
	while(1) {
		if(read(Fich->hdF, &E, sizeof(E)) <= 0)
			break;
			
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
