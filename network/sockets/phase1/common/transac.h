#ifndef __TRANSAC_H
#define __TRANSAC_H

	typedef enum {
		ERROR		= 0x00,
		HEADER		= 0x01,
		RESERVATION	= 0x02,
		BUY		= 0x03,
		EXIT		= 0x04
		
	} action_t;

	typedef struct transac_t {
		long ip;
		long port;
		long transac_id;
		long heure;
		long placelibre;
		long numticket;
		action_t action;
		
	} transac_t;
	
#endif
