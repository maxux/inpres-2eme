#ifndef __CHEMIN_HEADER
	#define __CHEMIN_HEADER
	
	#include "metro_protocol.h"
	
	typedef struct global_t {
		int *mkey_id;
		
	} global_t;
	
	int Recherche(int NoeudCourant,int NoeudDestination,int TailleActuelle, int CheminSuivit[],int CheminOptimum[],int *TailleChemin);
	void AffChemin(pathway_t *pathway);
#endif
