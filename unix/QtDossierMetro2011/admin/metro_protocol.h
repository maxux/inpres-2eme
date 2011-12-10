#ifndef __METRO_PROTOCOL
	#define __METRO_PROTOCOL
	
	/* Communication Protocol */
	typedef enum {
		QRY_LOGIN		= 0x01,	// QRY for authenficate client
		ACK_LOGIN		= 0x0C,	// ACK for login
		
		QRY_LOGOUT		= 0x02,	// QRY for unauthentificate client
		
		QRY_SHUTDOWN		= 0x04,	// QRY for daemon's shutdown
		ACK_SHUTDOWN		= 0x14,	// ACK shutdown confirmed
		
		QRY_SEARCH		= 0x03,	// QRY for path finding
		ACK_SEARCH		= 0x10,	// ACK for path finding
		QRY_SEARCH_DATA		= 0x11,	// ACK for chemin, with data
		
		QRY_JOIN_GROUP		= 0x05,	// QRY client to join a process-group
		ACK_JOIN_GROUP		= 0x06,	// ACK for client process-group		
		
		QRY_PATHLIST		= 0x07,	// QRY for map Path List
		ACK_PATHLIST		= 0x08,	// ACK for Path List: list on reply
		
		QRY_NODESLIST		= 0x12,	// QRY for metro_nodes list
		ACK_NODESLIST		= 0x13,	// ACK for metro_nodes list
		
		QRY_LINESLIST		= 0x09,	// QRY for map Lines List
		ACK_LINESLIST		= 0x0A,	// ACK for Line List: list on reply
		
		ACK_PONG		= 0x0B,	// ACK for a Ping Query (via sig)		
		
		ERR_DENIED		= 0x0D,	// ERR from server. Client is denied.
		
		QRY_ADMIN_LOGIN		= 0x0E,	// QRY for admin remote access
		ACK_ADMIN_LOGIN		= 0x0F,	// ACK for admin remote access
		
		QRY_ADMIN_MESSAGE	= 0x15,	// QRY send global admin message
		ACK_ADMIN_MESSAGE	= 0x16	// ACK message global sent
		
	} metro_protocol;
#endif

