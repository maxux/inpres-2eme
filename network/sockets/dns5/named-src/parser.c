#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "dns.h"
#include "named.h"
#include "parser.h"
#include "dns_parse.h"

char * config_copy_quoted(char *dst, char *src) {
	size_t len;
	
	if(*src == '"') {
		len = strlen(src) - 2;
		
		strncpy(dst, src + 1, len);
		
		/* Stripping final ';'  */
		if(dst[len - 1] == '"')
			len--;
			
		dst[len] = '\0';
		
	} else strcpy(dst, src);
	
	return dst;
}

void config_handle_acl(config_t *this) {
	while(this && this->level == 1) {
		printf("[ ] Parser: ACL -> %s\n", this->text);
		this = this->next;
	}
}

char * config_handle_domainfilename(config_t *this) {
	while(this) {
		if(!strncmp(this->text, "file", 4))
			return this->next->text;
		
		this = this->next;
	}
	
	return NULL;
}

char * config_remove_comments(char *buffer) {
	char *in, *out;
	
	while((in = strstr(buffer, "/*"))) {
		if(!(out = strstr(buffer, "*/"))) {
			fprintf(stderr, "[-] Parser: error: comment not closed\n");
			exit(EXIT_FAILURE);
		}
		
		memset(in, ' ', (out + 2) - in);
	}
	
	return buffer;
}

void debug_domaine(domaine_md_t *d) {
	size_t i;
	
	while(d) {
		printf("[D] DEBUG: DOMAIN: %s (File: %s)\n", d->nom, d->fichier);
		
		for(i = 0; i < d->nbrecord; i++)
			printf("[D] DEBUG: record: <%s> %s <%s>\n", d->record[i].gauche, __dns_type(d->record[i].type), d->record[i].droite);
			
		d = d->next;
	}
}

void debug_netid(netid_md_t *d) {
	size_t i;
	
	while(d) {
		printf("[D] DEBUG: NETID: %d.%d.%d.%d (File: %s)\n", d->o1, d->o2, d->o3, d->o4, d->fichier);
		
		for(i = 0; i < d->nbrecord; i++)
			printf("[D] DEBUG: record: <%s> %s <%s>\n", d->record[i].gauche, __dns_type(d->record[i].type), d->record[i].droite);
			
		d = d->next;
	}
}



domaine_md_t * config_handle_view(config_t *this, char *view) {
	domaine_md_t *domain = NULL, *new = NULL;
	char *temp;

	while(this && this->level > 0) {
		if(!strcmp(this->text, "zone")) {
			new = (domaine_md_t*) malloc(sizeof(domaine_md_t));
			if(!new) {
				fprintf(stderr, "[-] Parser: error: cannot allocate memory\n");
				exit(EXIT_FAILURE);
			}
	
			printf("[ ] Parser: VIEW: %s -> ZONE: %s\n", view, this->next->text);
			config_copy_quoted(new->nom, this->next->text);
			new->record = NULL;
			
			if((temp = config_handle_domainfilename(this->next)))
				config_copy_quoted(new->fichier, temp);
			
			/* Chaining */
			new->next = domain;
			domain    = new;
		}
		
		this = this->next;
	}
	
	return domain;
}

void config_grab_netid_ip(netid_md_t *netid, char *name, char *index) {
	int *list[] = {&netid->o1, &netid->o2, &netid->o3, &netid->o4};
	int i;
	
	for(i = 0; i < 4; i++)
		*list[i] = 0;
	
	i = 0;
	while(index-- > name) {
		while(*index != '.' && index != name)
			index--;
		
		if(*index == '.')
			*(list[i++]) = atoi(index + 1);
			
		else *(list[i++]) = atoi(index);
	}
}

netid_md_t * config_extract_netid(domaine_md_t **domaine) {
	domaine_md_t *dom = *domaine, *temp, *prev = NULL;
	netid_md_t *netid = NULL, *new = NULL;
	char *addr;
	
	while(dom) {
		if((addr = strstr(dom->nom, ".in-addr.arpa"))) {
			new = (netid_md_t*) malloc(sizeof(netid_md_t));
			if(!new) {
				fprintf(stderr, "[-] Parser: error: cannot allocate memory\n");
				exit(EXIT_FAILURE);
			}
			
			new->nbrecord = dom->nbrecord;
			new->record   = dom->record;
			
			strcpy(new->fichier, dom->fichier);
			config_grab_netid_ip(new, dom->nom, addr);
			
			if(*domaine == dom) {
				*domaine = dom->next;
				
			} else prev->next = dom->next;
		
			/* Chaining */
			new->next = netid;
			netid     = new;
			
			temp = dom->next;
			free(dom);
			
			dom  = temp;
			
		} else {
			prev = dom;
			dom  = dom->next;
		}
	}
	
	return netid;
}

void config_parse_records(domaine_md_t *node) {
	FILE *fp;
	char file[256], buffer[512];
	char host[256], type[32], value[256];
	size_t i;
	
	while(node) {
		node->nbrecord = 0;
		i = 0;
		
		sprintf(file, "%s/%s", PREFIX, node->fichier);
		printf("[ ] Parser: zone: init: %s\n", file);
		
		fp = fopen(file, "r");
		if(!fp)
			diep(file);
		
		while(fgets(buffer, sizeof(buffer), fp))
			node->nbrecord++;
		
		node->record = (records_md_t*) malloc(sizeof(records_md_t) * node->nbrecord);
		if(!node->record)
			diep("malloc");
		
		rewind(fp);
		
		while(fgets(buffer, sizeof(buffer), fp)) {
			if(sscanf(buffer, "%s IN %s %s", host, type, value) == 3) {
				printf("[ ] Parser: zone: host <%s> type <%s> value <%s>\n", host, type, value);
				
				if(!strcmp(type, "A")) {
					node->record[i].type = A;
					goto next;
				
				} else if(!strcmp(type, "NS")) {
					node->record[i].type = NS;
					goto next;
					
				} else if(!strcmp(type, "CNAME")) {
					node->record[i].type = CNAME;
					goto next;
					
				} else if(!strcmp(type, "PTR")) {
					node->record[i].type = PTR;
					
					next:
						strcpy(node->record[i].gauche, host);
						strcpy(node->record[i].droite, value);
						i++;
					
				} else node->nbrecord--;
			} else node->nbrecord--;
		}
		
		node->record = (records_md_t*) realloc(node->record, sizeof(records_md_t) * node->nbrecord);
		node = node->next;
	}
}

namedconf_md_t *config_parse(config_t *config) {
	config_t *this = config;
	namedconf_md_t *named;

	/* Allocating */	
	named = (namedconf_md_t*) malloc(sizeof(namedconf_md_t));
	if(!named)
		diep("malloc");
	
	named->domaine = NULL;
	named->netid   = NULL;
	
	while(this) {
		if(this->level == 0) {
			if(!strncmp(this->text, "acl", 3)) {
				this = this->next;
				printf("[ ] Parser: ACL: %s\n", this->text);
				config_handle_acl(this->next);
				
				continue;
			}
			
			if(!strncmp(this->text, "view", 4)) {
				this = this->next;
				printf("[ ] Parser: VIEW: %s\n", this->text);
				named->domaine = config_handle_view(this->next->next, this->text);
				config_parse_records(named->domaine);
				
				named->netid   = config_extract_netid(&named->domaine);
				continue;
			}
		}
		
		this = this->next;
	}
	
	return named;
}

config_t * config_append(char *text, int level) {
	config_t *this;
	unsigned int i, max;
	
	this = (config_t*) malloc(sizeof(config_t));
	this->level = level;
	this->next  = NULL;
	
	max = strlen(text);
	
	this->text = (char*) malloc(sizeof(char) + max + 1);
	strcpy(this->text, text);
	
	/* Lower case all */
	for(i = 0; i < max; i++)
		*(this->text+i) = tolower(*(text + i));
	
	return this;
}

namedconf_md_t * config_build(char *buff) {
	namedconf_md_t *named;
	char current[64], *buffer;
	int len = 0, level = 0, i;
	
	config_t *config, *last, *this;
	
	config = (config_t*) malloc(sizeof(config_t));
	config->next = NULL;
	
	config->level = -1;
	config->text  = NULL;
	
	last = config;
	
	/* Copy buffer pointer */
	buffer = buff;
	
	/* Parsing */
	while(sscanf(buffer, "%s%n", current, &len) == 1) {
		buffer += len;
		
		if(*current == '{') {
			level++;
			continue;
		}
		
		if(*current == '}') {
			level--;
			continue;
		}
		
		last->next = config_append(current, level);
		last = last->next;
		
		// printf("<%s>\n", current);
	}
	
	this = config->next;
	while(this) {
		printf("[ ] Parser: DEBUG: +");
		
		for(i = 0; i < this->level; i++)
			printf("---");
			
		printf("> %s\n", this->text);
		this = this->next;
	}
	
	named = config_parse(config->next);
	
	return named;
}

namedconf_md_t * lecture_namedconf_md(char *filename) {
	FILE *fp;
	char buffer[65535];
	long size;
	namedconf_md_t *named = NULL;
	
	/* Opening file */
	if(!filename)
		filename = NAMED_DOT_CONF;
		
	printf("[+] Parser: %s\n", filename);
	
	fp = fopen(filename, "r");
	if(!fp) {
		perror(filename);
		return NULL;
	}
	
	/* Scanning file */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	printf("[ ] Parser: config length: %ld\n", size);
	
	rewind(fp);
	
	/* Loading file */
	if(fread(buffer, size, 1, fp) != 1) {
		fclose(fp);
		
		perror("fread");
		return NULL;
	}
	
	/* printf("%s\n", buffer); */
	fclose(fp);
	
	/* Parsing */
	config_remove_comments(buffer);
	named = config_build(buffer);
	
	printf("[+] Parser: %s parsed. No error.\n", filename);
	
	return named;
}
