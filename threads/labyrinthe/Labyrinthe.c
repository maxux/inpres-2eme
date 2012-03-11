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

pthread_t tHero, tEvent, tPorte, tMaitreCles;
pthread_mutex_t mutexDestination, mutexNbCles;
pthread_cond_t condDestination, condNbCles;

int nbCles = 0;
int heroPix = 0;

// Hero's flags
hero_flags_t heroFlags = 0;

// Temporary
int cache;
int porteCle = 0;

void diep(char *s) {
	perror(s);
	exit(1);
}

int debug_speed = 1;

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void signal_handler(int signal) {
	switch(signal) {
		case SIGUSR1:
			printf("[+] Key Action (Thread ID: %d)\n", (int) pthread_self());
			
			/* Updating Map */
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			pthread_mutex_lock(&mutexTab);
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			tab[position_hero.C][position_hero.L] = CLE;
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			pthread_mutex_unlock(&mutexTab);
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			
			/* Updating cache */
			cache    = CLE;
			porteCle = 0;
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			/* Removing key from pix */
			printf("[ ] Hero pix (before): %d\n", heroPix);
			heroPix &= ~CLE_ID;
			printf("[ ] Hero pix (after): %d\n", heroPix);
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
			/* Redraw */
			// EffaceCarre(position_hero.L, position_hero.C);
			// DessineSprite(position_hero.L, position_hero.C, heroPix);
			printf("[ ] Hero Position: %d,%d\n", position_hero.C, position_hero.L);
		break;
		
		case SIGALRM:
			printf("[ ] ALARM on Thread ID: %d\n", (int) pthread_self());
		break;
	}
}

int main(void) {	
	sigset_t sigset;
	
	srand((unsigned)time(NULL));

	printf("[+] Initializing game\n");

	if(OuvrirGrilleSDL(NB_LIGNES, NB_COLONNES, 40, "Labyrinthe") < 0) {
		fprintf(stderr, "[-] Cannot initialize SDL\n");
		return 1;
	}
	
	
	/*
	 * DEBUG ENV
	 */
	if(getenv("DEBUG_GAME"))
		debug_speed = 20;

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
	
	/* Init Signals */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	signal_intercept(SIGUSR1, signal_handler);
	
	if(pthread_create(&tHero, NULL, threadHero, NULL))
		diep("[-] pthread_create");
	
	/* Clearing */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	
	/* Spawning Event Thread */
	printf("[+] Spawning threadEvent\n");
	if(pthread_create(&tEvent, NULL, threadEvent, NULL))
		diep("[-] pthread_create");
	
	
	/* Spawning maitreCles Thread */
	printf("[+] Spawning maitreCles\n");
	if(pthread_create(&tMaitreCles, NULL, threadMaitreCles, NULL))
		diep("[-] pthread_create");
	
	
	/* Spawning Porte Thread */
	printf("[+] Spawning Porte\n");
	
	sigfillset(&sigset);
	sigdelset(&sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	signal_intercept(SIGALRM, signal_handler);
	
	if(pthread_create(&tPorte, NULL, threadPorte, NULL))
		diep("[-] pthread_create");
	
	/* Clearing */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	
	/* Waiting end of game */
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
	struct timespec ts;
	
	printf("[+] Hero ID: %d\n", (int) pthread_self());
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
	
	/* Init Sleep Time */
	ts.tv_sec = 0;
	ts.tv_nsec = 500000000 / debug_speed;
	
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
			
			nanosleep(&ts, NULL);
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
				if((debug_speed != 1) || (position_hero.C == event.colonne || position_hero.L == event.ligne)) {
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
						
						pthread_kill(tHero, SIGUSR1);
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

void * threadPorte(void *dummy) {
	unsigned int i;
	char level_full;
	
	printf("[+] Porte ID: %d\n", (int) pthread_self());

	while(1) {	
		pthread_mutex_lock(&mutexTab);
		
		/* Flag to 1 */
		level_full = 1;
		
		for(i = 0; i < sizeof(positionStatues) / sizeof(POSITION); i++) {
			if(tab[positionStatues[i].L][positionStatues[i].C] != CLE) {
				/* Clearing flag */
				level_full = 0;
				break;
			}
		}
		
		pthread_mutex_unlock(&mutexTab);
		
		/* Checking flag */
		if(level_full)
			printf("___ LEVEL COMPLETED ___\n");
		
		/* Waiting next time */
		alarm(3);
		sleep(3); //
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
