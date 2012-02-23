#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "GrilleSDL.h"
#include "AStar.h" 

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
={ {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
   {1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,1},
   {1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1},
   {1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
   {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,5,5,5,0,1},
   {1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,5,5,5,0,2},
   {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,5,5,5,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
   {1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1},
   {1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1},
   {1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
  
void ChargementImages();
void DessineGrilleBase();
void Exemple();           // Fonction à supprimer

typedef struct position
{
  int L;
  int C;
} POSITION;

POSITION positionStatues[] = {{1,1},{1,13},{13,1},{13,13},{7,7},{1,7},{7,1},{7,13},{13,7}};
POSITION positionSerrures[] = {{6,15},{6,16},{6,17},{7,15},{7,16},{7,17},{8,15},{8,16},{8,17}};

///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
  srand((unsigned)time(NULL));

  // Ouverture de la grille de jeu (SDL)
  printf("Ouverture de la grille de jeu\n");
  fflush(stdout);
  if (OuvrirGrilleSDL(NB_LIGNES,NB_COLONNES,40,"Labyrinthe") < 0)
  {
    printf("Erreur de OuvrirGrilleSDL\n");
    fflush(stdout);
    exit(1);
  }

  // Definition des sprites et de l'image de fond
  ChargementImages();
  DessineGrilleBase();

  // Exemple d'utilisation des libraires GrilleSDL et AStar
  Exemple();

  // Fermeture de la grille de jeu (SDL)
  printf("Fermeture de la grille..."); fflush(stdout);
  FermerGrilleSDL();
  printf("OK\n"); fflush(stdout);

  exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChargementImages()
{
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
void DessineGrilleBase()
{
  int i,j;

  for (i=0 ; i<NB_LIGNES ; i++)
    for (j=0 ; j<NB_COLONNES ; j++)
    {
      if (tab[i][j] == MUR) DessineSprite(i,j,MUR);
      if (tab[i][j] == PORTE) DessineSprite(i,j,PORTE);
      if (tab[i][j] == SERRURE) DessineSprite(i,j,SERRURE);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void Exemple()
{
  EVENT_GRILLE_SDL event;

  // Exemple d'utilisation des libraires GrilleSDL et AStar

  CASE depart,arrivee;  // structure définie dans AStar.h
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

