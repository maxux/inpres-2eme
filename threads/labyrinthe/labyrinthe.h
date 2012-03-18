#ifndef __LABYRINTHE_MAIN
	#define __LABYRINTHE_MAIN
	
	#define NB_LIGNES   15
	#define NB_COLONNES 20

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


	#define AVEC_ID		  128
	
	#define HERO_DOS          31
	#define HERO_FACE_SANS    32
	#define HERO_FACE_AVEC    AVEC_ID | HERO_FACE_SANS
	#define HERO_GAUCHE_SANS  33
	#define HERO_GAUCHE_AVEC  AVEC_ID | HERO_GAUCHE_SANS
	#define HERO_DROITE_SANS  34
	#define HERO_DROITE_AVEC  AVEC_ID | HERO_DROITE_SANS

	#define GARDE_FACE        51
	#define GARDE_DOS         52
	#define GARDE_GAUCHE      53
	#define GARDE_DROITE      54

	#define STATUE_DOS          61
	#define STATUE_FACE_SANS    62
	#define STATUE_FACE_AVEC    AVEC_ID | STATUE_FACE_SANS
	#define STATUE_GAUCHE_SANS  63
	#define STATUE_GAUCHE_AVEC  AVEC_ID | STATUE_GAUCHE_SANS
	#define STATUE_DROITE_SANS  64
	#define STATUE_DROITE_AVEC  AVEC_ID | STATUE_DROITE_SANS

	#define ZERO                100
	#define UN                  101
	#define DEUX                102
	#define TROIS               103
	#define QUATRE              104
	#define CINQ                105
	#define SIX                 106
	#define SEPT                107
	#define HUIT                108
	#define NEUF                109
	#define VIES                111
	#define COEUR               112
	#define NIVEAU              113
	#define GAMEOVER            114
	#define SCORE               115

	#define BAS          1
	#define HAUT         2
	#define GAUCHE       3
	#define DROITE       4
	
	typedef enum hero_flags_t {
		NEW_DEST = 1,
		
	} hero_flags_t;
	
	typedef enum score_flags_t {
		GOT_NEW_KEY = 1,
		GOT_OLD_KEY = 2,
		PUT_KEY     = 4,
		GOT_PERLE   = 8,
		GOT_TRESOR  = 16
		
	} score_flags_t;
	
	typedef struct position_t {
		int L;
		int C;
		
	} position_t;
	
	typedef struct s_statue {
		position_t depart;
		position_t position;
		int indice;
		
	} S_STATUE;
	
	typedef struct s_personnage {
		int whoami;
		char porteCle;
		int cache;
		
	} S_PERSONNAGE;
	
	extern int tab[NB_LIGNES][NB_COLONNES];

	#define STATUES_COUNT		9
	#define SERRURES_COUNT		9
	
	extern position_t positionStatues[STATUES_COUNT];
	extern position_t positionSerrures[SERRURES_COUNT];
		
	extern position_t position_hero;
	extern position_t destination_hero;
	
	extern position_t position_porte;
	
	/* Tasks */
	extern position_t ListeTaches[10];
	extern int indiceInsertion;
	extern int indiceExtraction;
	
	extern float delay;
	extern short nbStatue;

	/* Threads/Mutex/Conditions */
	extern pthread_key_t spec_key;
	extern pthread_t tHero, tEvent, tPorte, tMaitreCles, tStatues[9], tGardePorte, tScore, tBonus;
	extern pthread_cond_t condDestination, condNbCles, condListeTaches, condStatueReady, condScore;
	extern pthread_mutex_t mutexDestination, mutexHero, mutexNbCles, mutexTab, mutexHeroPix, mutexStatueReady, mutexScoreFlags,
			       mutexPosition, mutexListeTaches, mutexIndiceInsertion, mutexIndiceExtraction, mutexScore;
	
	extern S_STATUE *__s_statue_debug[9];

	extern int nbCles;

	/* Hero's flags */
	extern hero_flags_t heroFlags;
	
	/* Score flags */
	extern score_flags_t scoreFlags;


	extern int heroPix;
	extern int debug_speed;
	
	
	/* Prototypes */
	int is_statue_position(position_t pos);
#endif
