#ifndef __NAMED_PARSER_H
	#define __NAMED_PARSER_H

	#define PREFIX			"../"
	#define NAMED_DOT_CONF		PREFIX "/etc/bind/named-delegate.conf"

	typedef struct config_t {
		int level;
		char *text;
	
		struct config_t *next;
	
	} config_t;

	namedconf_md_t * lecture_namedconf_md(char *filename);
#endif
