#include <stdio.h>
#include <stdlib.h>
#include "named.h"
#include "parser.h"

int main(void) {
	namedconf_md_t *named;
	
	if(!(named = lecture_namedconf_md(NULL)))
		return 1;
	
	debug_domaine(named->domaine);
	debug_netid(named->netid);
		
	return 0;
}
