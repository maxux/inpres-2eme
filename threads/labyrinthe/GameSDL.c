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

extern int tab[NB_LIGNES][NB_COLONNES];
extern int porteCle;

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

int get_hero_pix(CASE prev, CASE new) {
	/* printf("Previous: %d,%d\nNew: %d, %d\n", prev->L, prev->C, new->L, new->C); */
	
	/* Left or Right */
	if(prev.L == new.L) {
		if(prev.C < new.C)
			return (porteCle) ? HERO_DROITE_AVEC : HERO_DROITE_SANS;
			
		else return (porteCle) ? HERO_GAUCHE_AVEC : HERO_GAUCHE_SANS;
		
	/* Top or Bottom */
	} else {
		if(prev.L < new.L)
			return (porteCle) ? HERO_FACE_AVEC : HERO_FACE_SANS;
			
		else return HERO_DOS;
	}
	
	return HERO_FACE_SANS;
}
