#ifndef __CHEMIN_HEADER
	#define __CHEMIN_HEADER
	
	#define     CHEMIN

	#include "Commun.dat"
	#include "Donnee.dat"
	
	#define MESSAGE_KEY_ID		1342
	
	/* MESSAGES */
	#define MESSAGE_MAX_SIZE	512
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
	
	typedef struct global_t {
		int *mkey_id;
		
	} global_t;
	
	int Recherche(int NoeudCourant,int NoeudDestination,int TailleActuelle, int CheminSuivit[],int CheminOptimum[],int *TailleChemin);
	void AffChemin(int aChemin[],int TailleChemin);
#endif
