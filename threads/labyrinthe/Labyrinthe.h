#ifndef __LABYRINTHE_MAIN
	#define __LABYRINTHE_MAIN
	
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
	#define CLE_ID		  128
	
	#define HERO_DOS          31
	#define HERO_FACE_SANS    32
	#define HERO_FACE_AVEC    CLE_ID | HERO_FACE_SANS
	#define HERO_GAUCHE_SANS  33
	#define HERO_GAUCHE_AVEC  CLE_ID | HERO_GAUCHE_SANS
	#define HERO_DROITE_SANS  34
	#define HERO_DROITE_AVEC  CLE_ID | HERO_DROITE_SANS

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
	
	typedef enum hero_flags_t {
		NEW_DEST = 1,
		
	} hero_flags_t;
	
	typedef struct position {
		int L;
		int C;
		
	} POSITION;
	
	void * threadHero(void *dummy);
	void * threadEvent(void *dummy);
	void * threadMaitreCles(void *dummy);
	void * threadPorte(void *dummy);
	
	int is_statue_position(int L, int C);
#endif
