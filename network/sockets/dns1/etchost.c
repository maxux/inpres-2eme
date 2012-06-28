#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char * trim(char *str) {
	int i;
	
	i = strlen(str);
	if(str[i - 1] == '\n')
		str[i - 1] = '\0';
	
	return str;
}

char * skip_white(char *str) {
	int i = 0;
	
	while(*(str + i) && isspace(*(str + i)))
		i++;
	
	return str + i;
}

int word_len(char *str) {
	int i = 0;
	
	while(*str && !isspace(*(str++)))
		i++;
		
	return i;
}

char *ip_from_line(char *line, int *len) {
	int i = 0, k = 0;
	
	while(isspace(*(line+i)))
		i++;
	
	k = i;
	*len = 0;
	while(!isspace(*(line+(i++))))
		(*len)++;
	
	return line + k;
}

char * resolve(FILE *fp, char *name) {
	char buffer[1024], *ret = NULL;
	char *buf, *ip, *ip2;
	int len, i;
	
	rewind(fp);
	
	while(fgets(buffer, sizeof(buffer), fp)) {
		/* Skipping white spaces */
		buf = skip_white(buffer);
		
		/* Skipping comments */
		if(*buf == '#')
			continue;
		
		while(!isspace(*buf))
			buf++;
		
		/* Skipping IP */
		while(isspace(*buf))
			buf++;
		
		while((len = word_len(buf)) != 0) {
			if(!strncmp(buf, name, len)) {
				ip2 = ip_from_line(buffer, &i);
				ip = (char*) malloc(sizeof(char) * i + 1);
				
				strncpy(ip, ip2, i);
				ip[i] = '\0';
				
				printf("%s  >>  %s\n", name, ip);
				ret = (char*) 1;
			}
				
			
			buf += len;
			buf = skip_white(buf);
		}
	}
	
	return ret;
}

int main(int argc, char *argv[]) {
	char *file, reso[256], *out;
	FILE *fp;
	
	if(argc < 2) {
		fprintf(stderr, "Missing filename\n");
		return 1;
	}
	
	file = argv[1];
	
	fp = fopen(file, "r");
	if(!fp) {
		perror("fopen");
		return 1;
	}
	
	while(1) {
		printf("> ");
		while(!fgets(reso, sizeof(reso), stdin));
		trim(reso);
		
		if(!(out = resolve(fp, reso)))
			printf("Not found\n");
		
		printf("\n");
	}
	
	return 0;
}
