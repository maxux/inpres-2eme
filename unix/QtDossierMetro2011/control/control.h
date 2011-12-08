#ifndef __CONTROL_H
	#define __CONTROL_H
	
	
	/* MESSAGES */
	#define MESSAGE_MAX_SIZE	2048
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;

	/* METRO PROTOCOL */
	#define PROTO_QRY_LOGIN		0x01	// QRY for authenficate client
	#define PROTO_QRY_LOGOUT	0x02	// QRY for unauthentificate client
	#define PROTO_QRY_SEARCH	0x03	// QRY for path finding
	#define PROTO_QRY_SHUTDOWN	0x04	// QRY for daemon's shutdown
	
	#define PROTO_QRY_JOIN_GROUP	0x05	// QRY client to join a process-group
	#define PROTO_ACK_JOIN_GROUP	0x06	// ACK for client process-group
	
	#define PROTO_QRY_PATHLIST	0x07	// QRY for map Path List
	#define PROTO_ACK_PATHLIST	0x08	// ACK for Path List: list on reply
	
	#define PROTO_QRY_LINESLIST	0x09	// QRY for map Lines List
	#define PROTO_ACK_LINESLIST	0x0A	// ACK for Line List: list on reply
	
	/* IPC SETTINGS */
	#define MESSAGE_KEY_ID		1342
	
	#define SHARED_MEMORY_ID	1342
	#define SHARED_MEMORY_SIZE	2048
	
	/* DEBUGGING */
	#ifdef COLOR
		#define COLOR_BLUE	"\033[1;34m"
		#define COLOR_GREY	"\033[1;30m"
		#define COLOR_GREEN	"\033[1;32m"
		#define COLOR_RED	"\033[1;31m"
		#define COLOR_NONE	"\033[0m"
	#else
		#define COLOR_BLUE	""
		#define COLOR_GREY	""
		#define COLOR_GREEN	""
		#define COLOR_RED	""
		#define COLOR_NONE	""
	#endif
	
	/* Printing Message */
	#define __debug(...)	{ fprintf(stderr, __VA_ARGS__); \
				fprintf(stderr, COLOR_NONE); }
	
	/* Setting Colors */
	/* Classic Debug */
	#define debug(...)	{ fprintf(stderr, COLOR_GREY); \
				__debug(__VA_ARGS__); }
	
	/* Critical Debug */	
	#define debugc(...)	{ fprintf(stderr, COLOR_RED); \
				__debug(__VA_ARGS__); }
	
	
	/* CODE PROTOTYPES */
	void sighandler(int signal);
	int signal_intercept(int signal, void (*function)(int));
	void stopping_server();
	
	/* METRO DATA TYPES */
	typedef struct station_t {
		char station[20];	/* Nom de la station */
		int L; 	/* Ligne */
		int C;	/* Colonne */
		
	} station_t;

	typedef struct position_t {
		int N; // Numero de la station
		int L; // Ligne
		int C; // Colonne
	} position_t;
		
	typedef struct {
		position_t position[15];
		short couleur;
		
	} ligne_legacy_t;
	
	/* QT Color Correspondances */
	#define LEGACY_COLOR_BLUE		0x00
	#define LEGACY_COLOR_RED		0x01
	#define LEGACY_COLOR_YELLOW		0x02
	#define LEGACY_COLOR_GREEN		0x03
	#define LEGACY_COLOR_WHITE		0x04
	#define LEGACY_COLOR_BLACK		0x05
	#define LEGACY_COLOR_EOF		0xFF
	
	
	
	/* SYSTEM GLOBAL */
	typedef struct global_t {
		char running;
		int *mkey_id;
		struct client_table_t **clients_head;
		char *shm;
		
	} global_t;
#endif
