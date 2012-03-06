#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "GrilleSDL.h"
#include "AStar.h" 
#include "Labyrinthe.h"
#include "GameSDL.h"

int tab[NB_LIGNES][NB_COLONNES]
={ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1}, 
   {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1}, 
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 5, 5, 5, 0, 1}, 
   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 5, 5, 5, 0, 2}, 
   {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 5, 5, 5, 0, 1}, 
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1}, 
   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1}, 
   {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

pthread_mutex_t mutexTab;

POSITION positionStatues[] = {{1,1}, {1,13}, {13,1}, {13,13}, {7,7}, {1,7}, {7,1}, {7,13}, {13,7}};
POSITION positionSerrures[] = {{6,15}, {6,16}, {6,17}, {7,15}, {7,16}, {7,17}, {8,15}, {8,16}, {8,17}};
	
CASE position_hero    = {0, 16};
CASE destination_hero = {0, 16};

pthread_t tHero, tEvent;
pthread_mutex_t mutexDestination;
pthread_cond_t condDestination;

pthread_t tMaitreCles;
pthread_mutex_t mutexNbCles;
pthread_cond_t condNbCles;
int nbCles = 0;

// Hero's flags
hero_flags_t heroFlags = 0;

// Temporary
int cache;
int porteCle = 0;

void diep(char *s) {
	perror(s);
	exit(1);
}

int main(void) {	
	srand((unsigned)time(NULL));

	printf("[+] Initializing game\n");

	if(OuvrirGrilleSDL(NB_LIGNES, NB_COLONNES, 40, "Labyrinthe") < 0) {
		fprintf(stderr, "[-] Cannot initialize SDL\n");
		return 1;
	}

	// Definition des sprites et de l'image de fond
	ChargementImages();
	DessineGrilleBase();
	
	/* Initializing game */
	if(pthread_mutex_init(&mutexTab, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	/* Randomizing main characters position */
	position_hero.L    = (rand() % 2) ? 5 : 9;
	destination_hero.L = position_hero.L;
	cache 		   = VIDE;
	
	/* Threading threadHero */
	printf("[+] Spawning Hero's thread\n");
	if(pthread_create(&tHero, NULL, threadHero, NULL))
		diep("[-] pthread_create");
	
	printf("[+] Spawning threadEvent\n");
	if(pthread_create(&tEvent, NULL, threadEvent, NULL))
		diep("[-] pthread_create");
	
	printf("[+] Spawning maitreCles\n");
	if(pthread_create(&tMaitreCles, NULL, threadMaitreCles, NULL))
		diep("[-] pthread_create");
	
	pthread_join(tHero, NULL);

	// Fermeture de la grille de jeu (SDL)
	printf("[+] Closing map...\n");
	FermerGrilleSDL();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void * threadHero(void *dummy) {
	int valeursAutorisees[] = {VIDE, CLE};
	CASE *chemin = NULL;
	int nbCases, i;
	int heroPix;
	
	printf("[+] Spawning hero at (%d,%d)\n", position_hero.C, position_hero.L);
	
	/* Writing Hero's position */
	pthread_mutex_lock(&mutexTab);
	tab[position_hero.C][position_hero.L] = HERO;
	pthread_mutex_unlock(&mutexTab);
	
	/* Drawing */
	DessineSprite(position_hero.L, position_hero.C, HERO_FACE_SANS);
	
	/* Initializing mutex */
	pthread_mutex_init(&mutexDestination, NULL);
	pthread_mutex_lock(&mutexDestination);
	
	while(1) {
		printf("[ ] Hero: waiting event...\n");
		
		if(!(heroFlags & NEW_DEST))
			pthread_cond_wait(&condDestination, &mutexDestination);
		
		/* Accepting new destination */
		heroFlags &= ~NEW_DEST;
		printf("[+] Hero: new position request: %d,%d\n", destination_hero.C, destination_hero.L);
		
		/* Checking destination */
		switch(tab[destination_hero.L][destination_hero.C]) {
			case VIDE:
			case SERRURE:
			case CLE:
				break;
			
			default:
				printf("[-] Case denied\n");
				continue;
		}
		
		/* Path finding */
		nbCases = RechercheChemin(&tab[0][0], NB_LIGNES, NB_COLONNES, valeursAutorisees, sizeof(valeursAutorisees) / sizeof(int), position_hero, destination_hero, &chemin);
		
		if(!nbCases || !chemin) {
			printf("[-] Cannot reach destination\n");
			continue;
		}
		
		// pthread_mutex_lock(&mutexDestination);
		
		/* Moving */
		i = 0;		
		while(position_hero.C != destination_hero.C || position_hero.L != destination_hero.L) {
			if(heroFlags & NEW_DEST) {
				printf("[ ] New destination called. Cancelling this one...\n");
				break;
			}
			
			/* Updating tab */
			pthread_mutex_lock(&mutexTab);
			
			/* Updating cache and current position */
			tab[position_hero.L][position_hero.C] = cache;
			
			/* Updating UI */
			EffaceCarre(position_hero.L, position_hero.C);
			if(cache != VIDE)
				DessineSprite(position_hero.L, position_hero.C, cache);
			
			/* Saving next case */
			cache = tab[chemin[i].L][chemin[i].C];
			EffaceCarre(chemin[i].L, chemin[i].C);
			
			/* Checking if got a key */
			if(cache == CLE && !porteCle) {
				porteCle = 1;
				cache = VIDE;
			}
			
			/* Updating next case */
			tab[chemin[i].L][chemin[i].C] = HERO;
			
			pthread_mutex_unlock(&mutexTab);
			
			/* Writing UI */
			heroPix = get_hero_pix(position_hero, *(chemin + i));
			position_hero = chemin[i];
			
			printf("[ ] Hero position: %d,%d (destination: %d,%d)\n", position_hero.C, position_hero.L, destination_hero.C, destination_hero.L);
			DessineSprite(chemin[i].L, chemin[i].C, heroPix);
			
			usleep(500000);	// 500 ms
			i++;
		}
		
		printf("[+] Hero: destination reached\n");
		
		// pthread_mutex_unlock(&mutexDestination);
		
		/* Cleaning */
		if(chemin) {
			free(chemin);
			chemin = NULL;
		}
	}
		
	pthread_mutex_unlock(&mutexDestination);
	return dummy;
}

void * threadEvent(void *dummy) {
	EVENT_GRILLE_SDL event;
	int thiscase;
	
	while(1) {
		printf("[ ] Event: waiting event...\n");
		event = ReadEvent();
		
		switch(event.type) {
			case CLIC_GAUCHE:
				if(position_hero.C == event.colonne || position_hero.L == event.ligne) {
					pthread_mutex_lock(&mutexDestination);
					
					destination_hero.L = event.ligne;
					destination_hero.C = event.colonne;
					
					pthread_mutex_unlock(&mutexDestination);
					
					heroFlags |= NEW_DEST;
					
					pthread_cond_signal(&condDestination);
				}
			break;
			
			case CLAVIER:
				switch(event.touche) {
					case 'd':
					case 'D':
						if(!porteCle)
							continue;
						
						pthread_mutex_lock(&mutexTab);
						thiscase = tab[position_hero.L][position_hero.C];
						pthread_mutex_unlock(&mutexTab);
						
						if(cache != SERRURE)
							continue;
						
						printf("Deposer clé\n");
					break;
				}
			break;
			
			case CROIX:
				pthread_cancel(tHero);
			break;
		}
	}
	
	return dummy;
}

void * threadMaitreCles(void *dummy) {
	int L = 0, C = 0;
	
	/* DEBUG */
	/* unsigned int i;
	for(i = 0; i < sizeof(positionStatues) / sizeof(POSITION); i++)
		DessineSprite(positionStatues[i].L, positionStatues[i].C, TRESOR);
	*/
		
	/* Initializing mutex */
	pthread_mutex_init(&mutexNbCles, NULL);
	// pthread_mutex_lock(&mutexNbCles);
	
	while(1) {
		while(nbCles != 9) {
			pthread_mutex_lock(&mutexNbCles);
			nbCles++;
			pthread_mutex_unlock(&mutexNbCles);
			
			pthread_mutex_lock(&mutexTab);
			while(tab[L][C] != VIDE || is_statue_position(L, C)) {
				L = (rand() % 12) + 1;
				C = (rand() % 12) + 1;
				
				printf("[+] Spawning Key #%d: %d,%d\n", nbCles, L, C);
			}
			
			tab[L][C] = CLE;
			DessineSprite(L, C, CLE);
			pthread_mutex_unlock(&mutexTab);
		}
		
		pthread_cond_wait(&condNbCles, &mutexNbCles);
		printf("CLE CALL\n");
	}
	
	return dummy;
}

int is_statue_position(int L, int C) {
	unsigned int i;
	
	for(i = 0; i < sizeof(positionStatues) / sizeof(POSITION); i++)
		if(positionStatues[i].L == L && positionStatues[i].C == C)
			return 1;
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/* void Exemple()
{
  EVENT_GRILLE_SDL event;

  // Exemple d'utilisation des libraires GrilleSDL et AStar

    // structure définie dans AStar.h
  printf("Cliquez sur sur une case depart...\n");
  fflush(stdout);
  char ok = 0;
  while (!ok)
  {
    event = ReadEvent();  // Fonction bloquante attendant un evenement
    if (event.type == CLIC_GAUCHE)
    {
      depart.L = event.ligne;
      depart.C = event.colonne;
      if (tab[depart.L][depart.C] == VIDE) ok = 1;
    }
  }
  DessineSprite(depart.L,depart.C,HERO_FACE_AVEC);

  printf("Cliquez sur sur une case arrivee...\n");
  fflush(stdout);
  ok = 0;
  while (!ok)
  {
    event = ReadEvent();
    if (event.type == CLIC_GAUCHE)
    {
      arrivee.L = event.ligne;
      arrivee.C = event.colonne;
      if (tab[arrivee.L][arrivee.C] == VIDE) ok = 1;
    }
  }

  printf("Calcul et affichage du chemin...\n");
  fflush(stdout);
  int  valeursAutorisees[6];
  CASE *chemin;  // Futur chemin
  int  nbCases;
  valeursAutorisees[0] = VIDE;
  nbCases = RechercheChemin(&tab[0][0],NB_LIGNES,NB_COLONNES,valeursAutorisees,1,depart,arrivee,&chemin);
  if (nbCases > 0)
  {
    for (int i=0 ; i<nbCases ; i++)
      DessineSprite(chemin[i].L,chemin[i].C,HERO_FACE_SANS);
  }
  if (chemin) free(chemin);  // Ne pas oublier !!!
  
  printf("Cliquez sur la croix de la fenetre...\n");
  fflush(stdout);
  ok = 0;
  while (!ok)
  {  
    event = ReadEvent();
    if (event.type == CROIX) ok = 1;
  }

  return;
}
*/

