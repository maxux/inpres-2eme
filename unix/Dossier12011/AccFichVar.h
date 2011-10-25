#ifndef __ACCFICHVAR_H
	#define __ACCFICHVAR_H
	
	#define DATA_SIZE	20

	typedef struct {
		int hdF;
		int hdIndex;
		int Taille;
		int *Index;
		
	} FICHIERVAR;

	typedef struct {
		char Donnee[DATA_SIZE];
		int  Suivant;
		
	} ENREG;

	int FVOuvertureFichier(const char *, FICHIERVAR *);
	int FVFermetureFichier(FICHIERVAR *);
	int FVAjoutFichier(const char*, FICHIERVAR *);
	void FVAffiche(FICHIERVAR *);
	
	int UpdateFirstElement(int id, FICHIERVAR *Fich);
#endif
