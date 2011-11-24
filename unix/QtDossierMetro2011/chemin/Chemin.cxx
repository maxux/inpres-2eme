#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define     CHEMIN

#include "Commun.dat"
#include "Donnee.dat"

int Recherche(int NoeudCourant,int NoeudDestination,int TailleActuelle, int CheminSuivit[],int CheminOptimum[],int *TailleChemin);
void AffChemin(int aChemin[],int TailleChemin);

int idQ;

MESSAGE		MessageLu;

int main(int argc, char*argv[]) {
	int CheminSuivit[55];
	int CheminOptimum[55];
	int TailleChemin = 0;
	int rc;

	printf("Debug 1\n");

	// idQ = atoi(argv[1]);
	idQ = 1342;

	printf("Debug 1\n");
	/* if((rc = msgrcv(idQ, &MessageLu,sizeof(MessageLu) - sizeof(long),getpid(),0)) == -1) {
		fprintf(stderr, "%d %s %s\n", __LINE__,__FILE__, "Err. de msgrcv()");
		exit(1);
	} */
	// Trace("\t(Chemin %d) MessageRecu",getpid());

	printf("Debug 1\n");
	// Recherche(MessageLu.Message[0], MessageLu.Message[1], 0, CheminSuivit, &(CheminOptimum[1]), &TailleChemin);
	Recherche(2, 5, 0, CheminSuivit, &(CheminOptimum[1]), &TailleChemin);

	printf("Debug 1\n");
	CheminOptimum[0] = MessageLu.Message[0];
	AffChemin(CheminOptimum, TailleChemin + 1);

	int Taille = sizeof(long) + sizeof(int) + sizeof(pid_t) + sizeof(int) * (TailleChemin + 1);
	MESSAGE	*p = (MESSAGE *) malloc(Taille);
	
	printf("Debug 1\n");
	p->lType = MessageLu.idProcess;
	memcpy(p->Message, CheminOptimum, sizeof(int) * (TailleChemin + 1));
	
	printf("Sending\n");
	if(msgsnd(idQ, p, Taille - sizeof(long), 0) == -1) {
		// TraceErr(__LINE__,__FILE__,"Err. de msgsnd()");
		exit(1);
	}
	
	exit(0);
}


int Recherche(int NoeudCourant, int NoeudDestination, int TailleActuelle, int CheminSuivit[], int CheminOptimum[], int *TailleChemin) {
	int rc, Mrc, i;

	if(Noeud[NoeudCourant].Visite)
		return 0;
		
	if(NoeudCourant == NoeudDestination) {
		if(TailleActuelle < *TailleChemin || *TailleChemin == 0) {
			memcpy(CheminOptimum, CheminSuivit, TailleActuelle * sizeof(int));
			*TailleChemin = TailleActuelle;
			return 1;
		}
		
		return 0;
	}
	
	Noeud[NoeudCourant].Visite = 1;

	i = 0;
	while(i < 6 && Noeud[NoeudCourant].Suivant[i] != 0) {
		CheminSuivit[TailleActuelle] = Noeud[NoeudCourant].Suivant[i];
		rc = Recherche(Noeud[NoeudCourant].Suivant[i], NoeudDestination,TailleActuelle + 1, CheminSuivit,CheminOptimum,TailleChemin);
		
		if(rc != 0)
			Mrc = rc;
		
		i++;
	}
	
	Noeud[NoeudCourant].Visite = 0;
	return Mrc;
}

void AffChemin(int aChemin[],int TailleChemin) {
	int i = 0;
	char szBuffer[80];
	char szBuffer1[80];

	sprintf(szBuffer, "(%d)   ", TailleChemin);
	
	while(i < TailleChemin) {
		sprintf(szBuffer1, "%d ", aChemin[i++]);
		strcat(szBuffer, szBuffer1);
	}
	
	fprintf(stderr, "(Chemin %d): %s", getpid(), szBuffer);
	return;
}
