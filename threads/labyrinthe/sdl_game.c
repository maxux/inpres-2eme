#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "labyrinthe.h"
#include "astar.h" 
#include "sdl_grind.h"
#include "sdl_game.h"
#include "interface.h"

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
				DessineSprite(mk_position_t(i, j), MUR);
				
			if(tab[i][j] == PORTE)
				DessineSprite(mk_position_t(i, j), PORTE);
				
			if(tab[i][j] == SERRURE)
				DessineSprite(mk_position_t(i, j), SERRURE);
		}
	}

	DessineSprite(mk_position_t(1, 15), SCORE);
	DessineSprite(mk_position_t(2, 15), ZERO);
	DessineSprite(mk_position_t(2, 16), ZERO);
	DessineSprite(mk_position_t(2, 17), ZERO);
	DessineSprite(mk_position_t(2, 18), ZERO);

	DessineSprite(mk_position_t(12, 15), VIES);
	DessineSprite(mk_position_t(12, 17), HERO_FACE_SANS);
	DessineSprite(mk_position_t(12, 18), HERO_FACE_SANS);

	DessineSprite(mk_position_t(13, 15), NIVEAU);
	DessineSprite(mk_position_t(13, 17), ZERO);
	DessineSprite(mk_position_t(13, 18), UN);
}
