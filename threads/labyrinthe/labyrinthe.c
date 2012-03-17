#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h" 
#include "sdl_game.h"
#include "thread_hero.h"
#include "thread_door.h"
#include "thread_event.h"
#include "thread_keymaster.h"
#include "thread_statue.h"
#include "interface.h"

int tab[NB_LIGNES][NB_COLONNES] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 5, 5, 5, 0, 1}, 
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 5, 5, 5, 0, 0}, 
	{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 5, 5, 5, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1}, 
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
	
pthread_mutex_t mutexTab;

position_t positionStatues[STATUES_COUNT]   = {{1, 1},  {1, 13}, {13, 1}, {13, 13}, {7, 7},  {1, 7},  {7, 1},  {7, 13}, {13, 7}};
position_t positionSerrures[SERRURES_COUNT] = {{6, 15}, {6, 16}, {6, 17}, {7,  15}, {7, 16}, {7, 17}, {8, 15}, {8, 16}, {8, 17}};

/* Hero's Position */
pthread_mutex_t mutexPosition;
position_t position_hero    = {0, 16};

/* Hero's Destination */
pthread_mutex_t mutexDestination;
position_t destination_hero = {0, 16};

/* Door Position */
position_t position_porte = {7, 19};

pthread_mutex_t mutexNbCles;
pthread_cond_t condDestination, condNbCles;

int nbCles = 0;

float delay = 0.5;

/* Insertion/Extraction */
int indiceInsertion  = 0;
int indiceExtraction = 0;

pthread_mutex_t mutexIndiceInsertion, mutexIndiceExtraction;

// Hero's flags
hero_flags_t heroFlags = 0;

// Temporary
int cache;

int debug_speed = 1;

void diep(char *s) {
	perror(s);
	exit(1);
}

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
			set_tab(get_position_hero(), CLE);
			
			/* Updating cache */
			cache    = CLE;
			porteCle = 0;
			
			/* Removing key from pix */
			printf("[ ] Hero pix (before): %d\n", get_heropix());
			set_heropix(get_heropix() & ~AVEC_ID);
			printf("[ ] Hero pix (after): %d\n", get_heropix());
			
			/* Redraw */
			EffaceCarre(get_position_hero());
			DessineSprite(get_position_hero(), get_heropix());
			
			printf("[ ] Hero Position: %d,%d\n", position_hero.L, position_hero.C);
		break;
		
		case SIGALRM:
			printf("[ ] ALARM on Thread ID: %d\n", (int) pthread_self());
		break;
	}
}

int main(void) {	
	sigset_t sigset;
	int level = 1, i;
	S_STATUE *sid;
	
	srand((unsigned) time(NULL));

	printf("[+] Initializing game\n");

	if(OuvrirGrilleSDL(NB_LIGNES, NB_COLONNES, 40, "Labyrinthe") < 0) {
		fprintf(stderr, "[-] Cannot initialize SDL\n");
		return 1;
	}
	
	/* Init Door */
	tab[position_porte.L][position_porte.C] = PORTE;
	
	/*
	 * DEBUG ENV
	 */
	if(getenv("DEBUG_GAME"))
		debug_speed = 60;

	// Definition des sprites et de l'image de fond
	ChargementImages();
	DessineGrilleBase();
	
	/* Initializing game */
	if(pthread_mutex_init(&mutexTab, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexHero, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexPosition, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexDestination, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexHeroPix, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	/* Randomizing hero's position */
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
	
	for(i = 0; i < 4; i++) {
		sid = (S_STATUE*) malloc(sizeof(S_STATUE));
		
		sid->depart   = positionStatues[i];
		sid->position = sid->depart;
		sid->indice   = i;
		
		if(pthread_create(&tStatues[i], NULL, threadStatue, sid))
			diep("[-] pthread_create");
	}
	
	
	/* Waiting end of game */
	pthread_join(tHero, NULL);

	// Fermeture de la grille de jeu (SDL)
	printf("[+] Closing map...\n");
	FermerGrilleSDL();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////









int is_statue_position(position_t pos) {
	unsigned int i;
	
	for(i = 0; i < sizeof(positionStatues) / sizeof(position_t); i++)
		if(compare_position(pos, positionStatues[i]))
			return 1;
	
	return 0;
}
