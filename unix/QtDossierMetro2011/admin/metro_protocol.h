#ifndef __METRO_PROTOCOL
	#define __METRO_PROTOCOL
	
	/* Communication Protocol */
	typedef enum {
		QRY_ADMIN_LOGIN		= 0x0E,	// QRY for admin remote access
		ACK_ADMIN_LOGIN		= 0x0F,	// ACK for admin remote access
		
		ERR_DENIED		= 0x0D	// ERR from server. Client is denied.
		
	} metro_protocol;
#endif

