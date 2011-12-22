#ifndef __METRO_PROTOCOL
	#define __METRO_PROTOCOL
	
	#define METRO_MAX_LIGNE		20
	#define METRO_MAX_STATION	40
	#define METRO_MAX_POSITION	15
	
	#define METRO_FAKE_TIME		5 	/* 5 seconds */
	
	/* Communication Protocol */
	typedef enum {		
		QRY_NODESLIST		= 0x12,	// QRY for metro_nodes list
		ACK_NODESLIST		= 0x13,	// ACK for metro_nodes list
		
		QRY_LINESLIST		= 0x09,	// QRY for map Lines List
		ACK_LINESLIST		= 0x0A,	// ACK for Line List: list on reply
		
		QRY_METRO_MOVE		= 0x23, // QRY metro is on a new station
		ACK_METRO_MOVE		= 0x24	// ACK metro forwarded
		
	} metro_protocol;
	
	/* Lines Color List */
	typedef enum {
		LCOLOR_BLUE	= 0x00,
		LCOLOR_RED	= 0x01,
		LCOLOR_YELLOW	= 0x02,
		LCOLOR_GREEN	= 0x03,
		LCOLOR_WHITE	= 0x04,
		LCOLOR_BLACK	= 0x05,
		LCOLOR_EOF	= 0xFF
		
	} legacy_color;
	
	/* Stations List */
	typedef struct station_t {
		char station[20];	/* Nom de la station */
		int L; 	/* Ligne */
		int C;	/* Colonne */
		int enabled;
		
	} station_t;
	
	typedef struct position_t {
		int N; // Numero de la station
		int L; // Ligne
		int C; // Colonne
		
	} position_t;
	
	typedef struct {
		int visite;
		int suivant[6];
		
	} metro_nodes_t;
	
	/* Colored Schema (interface independant) */
	typedef struct ligne_legacy_t {
		position_t position[METRO_MAX_POSITION];
		legacy_color couleur;
		
	} ligne_legacy_t;
#endif
