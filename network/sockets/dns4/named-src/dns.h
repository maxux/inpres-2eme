#ifndef __DNS_HEADER
	#define __DNS_HEADER

	#define UDP_LIMIT	65535

	typedef enum dns_opcode_t {
		STANDARD	= 0
		
	} dns_opcode_t;
	char *__glob_dns_opcode_t[] = {"STANDARD"};

	typedef enum dns_query_t {
		QUERY		= 0x00,
		RESPONSE	= 0x01
		
	} dns_query_t;
	char *__glob_dns_query_t[] = {"QUERY", "RESPONSE"};

	typedef enum dns_type_t {	/* from RFC 1035                              */
		A	= 0x01,		/* a host address                             */
		NS	= 0x02,		/* an authoritative name server               */
		CNAME	= 0x05,		/* the canonical name for an alias            */
		PTR	= 0x0C,		/* a domain name pointer                      */
		
	} dns_type_t;
	
	typedef enum dns_type_t type_record_md_t;
	
	/* dns type printable debug */
	char *__glob_dns_type_t[] = {"UNKNOWN", "A", "NS", "MD", "MF", "CNAME", "SOA", "MB", "MG", "MR", "NULL", "WKS", "PTR", "HINFO", "MINFO", "MX", "TXT"};

	typedef enum dns_class_t {	/* from RFC 1035    */
		IN	= 0x01,		/* the Internet     */
		CS	= 0x02,		/* the CSNET class  */
		CH	= 0x03,		/* the CHAOS class  */
		HS	= 0x04		/* Hesiod [Dyer 87] */
		
	} dns_class_t;

	/* dns class printable debug */
	char *__glob_dns_class_t[] = {"UNKNOWN", "IN", "CS", "CH", "HS"};

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
