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
	
	typedef struct {
		int id;
		int length;
		
	} index_element_t;

	int FVOuvertureFichier(const char *NomFichier, FICHIERVAR *Fich);
	int FVFermetureFichier(FICHIERVAR *Fich);
	int FVAjoutFichier(const char *Info, FICHIERVAR *Fich);
	void FVAffiche(FICHIERVAR *Fich);
	
	int FVConsultation(int id, FICHIERVAR *Fich);
	int FVSuppression(int id, FICHIERVAR *Fich);
#endif
