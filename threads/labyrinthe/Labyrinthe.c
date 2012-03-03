#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "GrilleSDL.h"
#include "AStar.h" 
#include "Labyrinthe.h"

// Dimensions de la grille de jeu
#define NB_LIGNES   15
#define NB_COLONNES 20

// Macros utlisees dans le tableau tab mais pouvant aussi etre utlisees pour les sprites
#define VIDE         0
#define MUR          1
#define PORTE        2
#define HERO         3
#define CLE          4
#define SERRURE      5
#define GARDE        6
#define STATUE       7
#define TRESOR       8
#define PERLE        9
#define MEDUSE       10

// Macros associees aux sprites
#define HERO_FACE_AVEC    31
#define HERO_FACE_SANS    32
#define HERO_DOS          33
#define HERO_GAUCHE_AVEC  34
#define HERO_GAUCHE_SANS  35
#define HERO_DROITE_AVEC  36
#define HERO_DROITE_SANS  37

#define GARDE_FACE        51
#define GARDE_DOS         52
#define GARDE_GAUCHE      53
#define GARDE_DROITE      54

#define STATUE_FACE_AVEC    61
#define STATUE_FACE_SANS    62
#define STATUE_DOS          63
#define STATUE_GAUCHE_AVEC  64
#define STATUE_GAUCHE_SANS  65
#define STATUE_DROITE_AVEC  66
#define STATUE_DROITE_SANS  67

#define SCORE               100
#define UN                  101
#define DEUX                102
#define TROIS               103
#define QUATRE              104
#define CINQ                105
#define SIX                 106
#define SEPT                107
#define HUIT                108
#define NEUF                109
#define ZERO                110
#define VIES                111
#define COEUR               112
#define NIVEAU              113
#define GAMEOVER            114

// Macros assosiees au sens de deplacement des personnages
#define BAS          1
#define HAUT         2
#define GAUCHE       3
#define DROITE       4

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
  
void ChargementImages();
void DessineGrilleBase();
void Exemple();           // Fonction à supprimer

typedef struct position {
	int L;
	int C;
	
} POSITION;

POSITION positionStatues[] = {{1,1},{1,13},{13,1},{13,13},{7,7},{1,7},{7,1},{7,13},{13,7}};
POSITION positionSerrures[] = {{6,15},{6,16},{6,17},{7,15},{7,16},{7,17},{8,15},{8,16},{8,17}};

CASE position_hero    = {0, 16};
CASE destination_hero = {0, 16};

pthread_t tHero, tEvent;
pthread_mutex_t mutexTab;
pthread_mutex_t mutexDestination;
pthread_cond_t condDestination;

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
	
	/* Threading threadHero */
	printf("[+] Spawning Hero's thread\n");
	if(pthread_create(&tHero, NULL, threadHero, NULL)) {
		perror("[-] pthread_create");
		return 1;
	}
	
	printf("[+] Spawning threadEvent\n");
	if(pthread_create(&tEvent, NULL, threadEvent, NULL)) {
		perror("[-] pthread_create");
		return 1;
	}
	
	pthread_join(tHero, NULL);

	// Fermeture de la grille de jeu (SDL)
	printf("[+] Closing map...\n");
	FermerGrilleSDL();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChargementImages() {
	// Definition des sprites et de l'image de fond
	DessineImageFond("./images/Background.bmp");

	AjouteSpriteAFondTransparent(HERO_FACE_AVEC,"./images/HeroFaceAvecPerle.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_FACE_SANS,"./images/HeroFaceSansPerle.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_DOS,"./images/HeroDos.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_GAUCHE_AVEC,"./images/HeroGaucheAvecPerle.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_GAUCHE_SANS,"./images/HeroGaucheSansPerle.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_DROITE_AVEC,"./images/HeroDroiteAvecPerle.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HERO_DROITE_SANS,"./images/HeroDroiteSansPerle.bmp",255,255,255);

	AjouteSprite(MUR,"./images/Mur.bmp");
	AjouteSpriteAFondTransparent(PORTE,"./images/Porte.bmp",255,255,255);
	AjouteSpriteAFondTransparent(CLE,"./images/PerleRose.bmp",255,255,255);
	AjouteSpriteAFondTransparent(SERRURE,"./images/Serrure.bmp",255,255,255);
	AjouteSpriteAFondTransparent(TRESOR,"./images/Tresor.bmp",255,255,255);
	AjouteSpriteAFondTransparent(PERLE,"./images/PerleBleue.bmp",255,255,255);

	AjouteSpriteAFondTransparent(GARDE_FACE,"./images/GardeFace.bmp",255,255,255);
	AjouteSpriteAFondTransparent(GARDE_DOS,"./images/GardeDos.bmp",255,255,255);
	AjouteSpriteAFondTransparent(GARDE_GAUCHE,"./images/GardeGauche.bmp",255,255,255);
	AjouteSpriteAFondTransparent(GARDE_DROITE,"./images/GardeDroite.bmp",255,255,255);

	AjouteSpriteAFondTransparent(STATUE_FACE_AVEC,"./images/StatueFaceYeuxRouges.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_FACE_SANS,"./images/StatueFaceYeuxVerts.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_DOS,"./images/StatueDos.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_GAUCHE_AVEC,"./images/StatueGaucheYeuxRouges.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_GAUCHE_SANS,"./images/StatueGaucheYeuxVerts.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_DROITE_AVEC,"./images/StatueDroiteYeuxRouges.bmp",255,255,255);
	AjouteSpriteAFondTransparent(STATUE_DROITE_SANS,"./images/StatueDroiteYeuxVerts.bmp",255,255,255);

	AjouteSpriteAFondTransparent(SCORE,"./images/Score.bmp",255,255,255);
	AjouteSpriteAFondTransparent(ZERO,"./images/Zero.bmp",255,255,255);
	AjouteSpriteAFondTransparent(UN,"./images/Un.bmp",255,255,255);
	AjouteSpriteAFondTransparent(DEUX,"./images/Deux.bmp",255,255,255);
	AjouteSpriteAFondTransparent(TROIS,"./images/Trois.bmp",255,255,255);
	AjouteSpriteAFondTransparent(QUATRE,"./images/Quatre.bmp",255,255,255);
	AjouteSpriteAFondTransparent(CINQ,"./images/Cinq.bmp",255,255,255);
	AjouteSpriteAFondTransparent(SIX,"./images/Six.bmp",255,255,255);
	AjouteSpriteAFondTransparent(SEPT,"./images/Sept.bmp",255,255,255);
	AjouteSpriteAFondTransparent(HUIT,"./images/Huit.bmp",255,255,255);
	AjouteSpriteAFondTransparent(NEUF,"./images/Neuf.bmp",255,255,255);

	AjouteSpriteAFondTransparent(VIES,"./images/Vies.bmp",255,255,255);
	AjouteSpriteAFondTransparent(NIVEAU,"./images/Niveau.bmp",255,255,255);
	AjouteSpriteAFondTransparent(GAMEOVER,"./images/GameOver.bmp",255,255,255);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DessineGrilleBase() {
	int i,j;

	for(i = 0; i < NB_LIGNES; i++) {
		for(j = 0; j < NB_COLONNES; j++) {
			if(tab[i][j] == MUR)
				DessineSprite(i,j,MUR);
				
			if(tab[i][j] == PORTE)
				DessineSprite(i,j,PORTE);
				
			if(tab[i][j] == SERRURE)
				DessineSprite(i,j,SERRURE);
		}
	}

	DessineSprite(1,15,SCORE);
	DessineSprite(2,15,ZERO);
	DessineSprite(2,16,ZERO);
	DessineSprite(2,17,ZERO);
	DessineSprite(2,18,ZERO);

	DessineSprite(12,15,VIES);
	DessineSprite(12,17,HERO_FACE_SANS);
	DessineSprite(12,18,HERO_FACE_SANS);

	DessineSprite(13,15,NIVEAU);
	DessineSprite(13,17,ZERO);
	DessineSprite(13,18,UN);
}

void * threadHero(void *dummy) {
	int valeursAutorisees[] = {VIDE};
	CASE *chemin = NULL;
	int nbCases, i;
	
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
		pthread_cond_wait(&condDestination, &mutexDestination);
		
		printf("[+] Hero: new position request: %d,%d\n", destination_hero.C, destination_hero.L);
		
		/* Checking destination */
		if(tab[destination_hero.L][destination_hero.C] != VIDE || tab[destination_hero.L][destination_hero.C] != SERRURE) {
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
		
		i = 0;
		while(position_hero.C != destination_hero.C || position_hero.L != destination_hero.L) {
			EffaceCarre(position_hero.L, position_hero.C);
			position_hero = chemin[i];
			
			printf("[ ] Hero position: %d,%d (destination: %d,%d)\n", position_hero.C, position_hero.L, destination_hero.C, destination_hero.L);
			DessineSprite(chemin[i].L, chemin[i].C, HERO_FACE_SANS);
			
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
	
	while(1) {
		printf("[ ] Event: waiting event...\n");
		event = ReadEvent();
		
		switch(event.type) {
			case CLIC_GAUCHE:
				// pthread_mutex_lock(&mutexDestination);
				destination_hero.L = event.ligne;
				destination_hero.C = event.colonne;
				
				pthread_cond_signal(&condDestination);
			break;
			
			case CROIX:
				pthread_cancel(tHero);
			break;
		}
	}
	
	return dummy;
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

