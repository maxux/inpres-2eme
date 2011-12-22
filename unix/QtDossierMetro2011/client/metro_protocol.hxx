#ifndef __METRO_PROTOCOL
	#define __METRO_PROTOCOL
	
	#define METRO_MAX_STEP		32
	#define METRO_MAX_POSITION	15
	
	/* Communication Protocol */
	typedef enum {
		QRY_LOGIN		= 0x01,	// QRY for authenficate client
		ACK_SEARCH		= 0x10,	// ACK for path finding		
		
		QRY_LOGOUT		= 0x02,	// QRY for unauthentificate client
		QRY_SEARCH		= 0x03,	// QRY for path finding
		QRY_SHUTDOWN		= 0x04,	// QRY for daemon's shutdown
		
		QRY_JOIN_GROUP		= 0x05,	// QRY client to join a process-group
		ACK_JOIN_GROUP		= 0x06,	// ACK for client process-group
		
		QRY_PATHLIST		= 0x07,	// QRY for map Path List
		ACK_PATHLIST		= 0x08,	// ACK for Path List: list on reply
		
		QRY_LINESLIST		= 0x09,	// QRY for map Lines List
		ACK_LINESLIST		= 0x0A,	// ACK for Line List: list on reply
		
		ACK_PONG		= 0x0B,	// ACK for a Ping Query (via sig)
		
		ACK_LOGIN		= 0x0C,	// ACK for login
		ERR_DENIED		= 0x0D	// ERR from server. Client is denied.
		
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
	
	/* Colored Schema (QT) */
	typedef struct ligne_t {
		position_t position[15];
		QColor couleur;
		
	} ligne_t;
	
	/* Colored Schema (interface independant) */
	typedef struct ligne_legacy_t {
		position_t position[METRO_MAX_POSITION];
		legacy_color couleur;
		
	} ligne_legacy_t;
	
	typedef struct ask_pathway_t {
		pid_t client;
		int from;
		int to;
		
	} ask_pathway_t;
	
	typedef struct pathway_t {
		int nbstation;
		int step[METRO_MAX_STEP];
		
	} pathway_t;
#endif
