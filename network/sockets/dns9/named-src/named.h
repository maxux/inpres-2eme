#ifndef __NAMED_H
	#define __NAMED_H
	
	#define	BIND_ADDRESS	"127.0.0.1"
	#define BIND_PORT	10053
	
	typedef struct acl_md_t {
		char name[64];
		unsigned char mask[32];		/* Max 32 ipv4 */
		unsigned char nbip;
		
	} acl_md_t;
	
	typedef struct records_md_t {
		char gauche[40];
		enum dns_type_t type;
		char droite[40];
		
	} records_md_t;

	/* IN {A, CNAME} */
	typedef struct domaine_md_t {
		char nom[40];
		char fichier[40];
		
		struct records_md_t *record;
		size_t nbrecord;
		struct domaine_md_t *next;
		
	} domaine_md_t;
	
	/* IN PTR */
	typedef struct netid_md_t {
		int o1;
		int o2;
		int o3;
		int o4;
		
		char fichier[40];
		
		struct records_md_t *record;
		size_t nbrecord;
		struct netid_md_t *next;
		
	} netid_md_t;
	
	/* Global Config Listed */
	typedef struct namedconf_md_t {
		struct netid_md_t * netid;         /* IN PTR        */
		struct domaine_md_t * domaine;     /* IN {A, CNAME} */
		
	} namedconf_md_t;
	
	void diep(char *str);
#endif
