#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "sdl_grind.h"
#include "labyrinthe.h"
#include "astar.h" 
#include "sdl_game.h"
#include "thread_hero.h"
#include "thread_door.h"
#include "thread_event.h"
#include "thread_keymaster.h"
#include "thread_statue.h"
#include "thread_gardeporte.h"
#include "thread_score.h"
#include "thread_bonus.h"
#include "thread_niveau.h"
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
	// {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 4, 4, 4, 0, 1}, 
	// {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 4, 4, 4, 0, 0}, 
	// {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 4, 4, 4, 0, 1}, 
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
position_t position_hero;

/* Hero's Destination */
pthread_mutex_t mutexDestination;
position_t destination_hero = {0, 16};

/* Door Position */
position_t position_porte = {7, 19};

pthread_mutex_t mutexNbCles;
pthread_cond_t condDestination, condNbCles;

int nbCles = 0;
short nbStatue = 0;

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
S_STATUE *__s_statue_debug[9] = {NULL};
pthread_key_t spec_key;

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
	S_PERSONNAGE *me;
	int thiscase;
	
	switch(signal) {
		case SIGUSR1:
			if(!(me = pthread_getspecific(spec_key))) {
				fprintf(stderr, "[-] ERROR: getspecific failed\n");
				exit(1);
			}
			
			if(!me->porteCle)
				break;
			
			thiscase = get_tab(get_position_hero());
			
			if(me->cache != SERRURE)
				break;
			
			/* Updating Score */
			pthread_mutex_lock(&mutexScoreFlags);
			scoreFlags = PUT_KEY;
			
			pthread_cond_signal(&condScore);
						
			printf("[+] Key Action (Thread ID: %d)\n", (int) pthread_self());
			
			/* Updating Map */
			set_tab(get_position_hero(), CLE);
			
			/* Updating cache */			
			me->cache    = CLE;
			me->porteCle = 0;
			
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
		
		case SIGUSR2:
			if(!(me = pthread_getspecific(spec_key))) {
				fprintf(stderr, "[-] ERROR: getspecific failed\n");
				exit(1);
			}
			
			if(me->whoami == HERO) {
				printf("[-] Hero: I'm dead...\n");

				if(me->porteCle || me->cache == CLE)
					set_nbcle(get_nbcle() - 1);
					
				else if(me->porteCle && me->cache == CLE)
					set_nbcle(get_nbcle() - 2);
					
				pthread_cond_signal(&condNbCles);
				
				pthread_exit(NULL);
				
			} else if(me->whoami == STATUE) {
				pthread_exit(NULL);
			}
		break;
	}
}

void keyDestruc(void *value) {
	printf("[+] ThreadManager: Destructing...\n");
	free(value);
	// pthread_setspecific(spec_key, NULL);
}

int main(void) {	
	sigset_t sigset;
	int i, j;
	S_STATUE *sid;
	int life = 3;
	position_t lfpix[] = {{12, 17}, {12, 18}};
	struct timespec ts;
		
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
		
	if(pthread_mutex_init(&mutexStatueReady, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexScore, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexScoreFlags, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	if(pthread_mutex_init(&mutexNiveau, NULL))
		fprintf(stderr, "[-] Cannot initialize mutex\n");
	
	/* Init Specific Key */
	if(pthread_key_create(&spec_key, keyDestruc))
		fprintf(stderr, "[-] Cannot initialize key\n");
	
	/* Threading threadHero */
	printf("[+] Spawning Statues thread\n");
	
	/* Init Signals */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGUSR1);
	sigdelset(&sigset, SIGUSR2);
	sigdelset(&sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	signal_intercept(SIGUSR1, signal_handler);
	signal_intercept(SIGUSR2, signal_handler);
	signal_intercept(SIGALRM, signal_handler);
	
	/* Spawning Statues */
	nbStatue = 4;
	for(i = 0; i < nbStatue; i++) {
		sid = (S_STATUE*) malloc(sizeof(S_STATUE));
		
		sid->depart   = positionStatues[i];
		sid->position = sid->depart;
		sid->indice   = i;
		
		__s_statue_debug[i] = sid;
		
		if(pthread_create(&tStatues[i], NULL, threadStatue, sid))
			diep("[-] pthread_create");
	}
	
	/* Clearing SIG */
	/* sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR2);
	sigprocmask(SIG_BLOCK, &sigset, NULL); */
	
	/* Spawning Event Thread */
	printf("[+] Spawning threadEvent\n");
	if(pthread_create(&tEvent, NULL, threadEvent, NULL))
		diep("[-] pthread_create");
	
	/* Spawning Garde Thread */
	printf("[+] Spawning threadGarde\n");
	if(pthread_create(&tGardePorte, NULL, threadGardePorte, NULL))
		diep("[-] pthread_create");
	
	/* Spawning Score Thread */
	printf("[+] Spawning threadScore\n");
	if(pthread_create(&tScore, NULL, threadScore, NULL))
		diep("[-] pthread_create");
	
	/* Spawning Bonus Thread */
	printf("[+] Spawning threadBonus\n");
	if(pthread_create(&tBonus, NULL, threadBonus, NULL))
		diep("[-] pthread_create");
	
	printf("[+] Spawning threadNiveau\n");
	if(pthread_create(&tNiveau, NULL, threadNiveau, NULL))
		diep("[-] pthread_create");
	
	/* Spawning maitreCles Thread */
	printf("[+] Spawning maitreCles\n");
	if(pthread_create(&tMaitreCles, NULL, threadMaitreCles, NULL))
		diep("[-] pthread_create");
	
	
	/* Spawning Porte Thread */
	printf("[+] Spawning Porte\n");
	
	// sigfillset(&sigset);
	/* sigdelset(&sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	signal_intercept(SIGALRM, signal_handler); */
	
	if(pthread_create(&tPorte, NULL, threadPorte, NULL))
		diep("[-] pthread_create");
	
	/* Clearing */
	/* sigemptyset(&sigset);
	sigaddset(&sigset, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigset, NULL); */
	
	/* Hero */
	
	// sigfillset(&sigset);
	/* sigdelset(&sigset, SIGUSR1);
	sigdelset(&sigset, SIGUSR2);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	signal_intercept(SIGUSR1, signal_handler); */
	// signal_intercept(SIGUSR2, signal_handler);
	
	printf("[+] Spawning Hero\n");
	
	if(pthread_create(&tHero, NULL, threadHero, NULL))
		diep("[-] pthread_create");
	
	/* Clearing */
	/* sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sigprocmask(SIG_BLOCK, &sigset, NULL); */
	
	/* Waiting end of game */
	while(life) {
		pthread_join(tHero, NULL);
		
		printf("[+] Re-Spawning Hero\n");
		
		life--;
		printf("[+] Master: Life Remain: %d\n", life);
		printf("[ ] Master: Clearing Hero...\n");
		
		if(life > 0)
			EffaceCarre(lfpix[life - 1]);
		else break;
		
		printf("[+] Master: Respawning Hero...\b");
		
		pthread_mutex_unlock(&mutexPosition);
		pthread_mutex_unlock(&mutexDestination);
		
		if(pthread_create(&tHero, NULL, threadHero, NULL))
			diep("[-] pthread_create");
	}
	
	for(i = 0; i < nbStatue; i++)
		pthread_cancel(tStatues[i]);
	
	pthread_cancel(tGardePorte);
	pthread_cancel(tBonus);
	pthread_cancel(tScore);
	pthread_cancel(tPorte);
	pthread_cancel(tEvent);
	pthread_cancel(tNiveau);
	
	ts.tv_sec  = 5;
	ts.tv_nsec = 0;
	
	/* Mapping MUR */
	printf("Mapping...\n");
	
	for(i = 0; i < NB_LIGNES; i++) {
		position_hero.L = i;
		
		for(j = 0; j < NB_COLONNES; j++) {
			printf(".\n");
			position_hero.C = j;
			
			set_tab_nonblock(position_hero, MUR);
			
			EffaceCarre(position_hero);
			DessineSprite(position_hero, MUR);
		}
	} 
	printf("EO Mapping...\n");
	position_hero.L = 7;
	position_hero.C = 8;
	
	DessineSprite(position_hero, GAMEOVER);
	
	nanosleep(&ts, NULL);

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
