#ifndef __DNS_HEADER
	#define __DNS_HEADER

	#define UDP_LIMIT	65535

	typedef enum dns_opcode_t {
		STANDARD	= 0
		
	} dns_opcode_t;
	

	typedef enum dns_query_t {
		QUERY		= 0x00,
		RESPONSE	= 0x01
		
	} dns_query_t;
	
	typedef enum dns_type_t {	/* from RFC 1035                              */
		A	= 0x01,		/* a host address                             */
		NS	= 0x02,		/* an authoritative name server               */
		MD	= 0x03,		/* a mail destination (Obsolete - use MX)     */
		MF	= 0x04,		/* a mail forwarder (Obsolete - use MX)       */
		CNAME	= 0x05,		/* the canonical name for an alias            */
		SOA	= 0x06,		/* marks the start of a zone of authority     */
		MB	= 0x07,		/* a mailbox domain name (EXPERIMENTAL)       */
		MG	= 0x08,		/* a mail group member (EXPERIMENTAL)         */
		MR	= 0x09,		/* a mail rename domain name (EXPERIMENTAL)   */
		NUL	= 0x0A,		/* a null RR (EXPERIMENTAL)                   */
		WKS	= 0x0B,		/* a well known service description           */
		PTR	= 0x0C,		/* a domain name pointer                      */
		HINFO	= 0x0D,		/* host information                           */
		MINFO	= 0x0E,		/* mailbox or mail list information           */
		MX	= 0x0F,		/* mail exchange                              */
		TXT	= 0x10		/* text strings	                              */
		
	} dns_type_t;

	typedef enum dns_class_t {	/* from RFC 1035    */
		IN	= 0x01,		/* the Internet     */
		CS	= 0x02,		/* the CSNET class  */
		CH	= 0x03,		/* the CHAOS class  */
		HS	= 0x04		/* Hesiod [Dyer 87] */
		
	} dns_class_t;

	typedef struct dns_header_t {
		uint16_t id;
		uint8_t rd : 1;		/* recursion flag */
		uint8_t tc : 1;		/* trucated flag */
		uint8_t aa : 1;		/* authoritive Answer */
		
		dns_opcode_t opcode : 4;	/* operation */
		dns_query_t qr : 1;		/* query/Response Flag */
		
		uint8_t rcode : 4;	/* response Code */
		uint8_t cd : 1;		/* check flag */
		uint8_t ad : 1;		/* authenticated Data */
		uint8_t z : 1;		/* reserved */
		uint8_t ra : 1;		/* recursion Availide */
		uint16_t q_count;	/* question Count */
		uint16_t ans_count;	/* answer Count */
		uint16_t auth_count;	/* authority Count */
		uint16_t add_count;	
		
	} __attribute__((packed)) dns_header_t;

	typedef struct dns_question_t {
		dns_type_t qtype : 16;
		dns_class_t qclass : 16;
		
	} __attribute__((packed)) dns_question_t;
	
	typedef struct dns_rdata_t {
		uint16_t rtype;
		uint16_t rclass;
		uint32_t rttl;
		uint16_t data_len;
		
		
	} __attribute__((packed)) dns_rdata_t;
#endif
