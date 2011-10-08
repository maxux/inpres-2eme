#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int ouverture(const char *filename) {
	int fd;
	
	fd = open(filename, O_RDWR);
	if(fd == -1) {
		fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0664);
		if(fd == -1) {
			perror("creat:");
			exit(EXIT_FAILURE);
		}
	}
	
	return fd;
}

int ajout_data(int fd, const write_data_t *data) {
	lseek(fd, 0, SEEK_END);
	
	if(write(fd, data->name, strlen(data->name)) != (signed) strlen(data->name))
		printf("ERROR\n");
	
	if(write(fd, data->surname, strlen(data->surname)) != (signed) strlen(data->surname))
		printf("ERROR\n");
	
	return 0;
}

int lire_data(int fd) {
	char buffer[256];
	
	lseek(fd, 0, SEEK_SET);
	
	while(read(fd, buffer, sizeof(buffer)) == sizeof(buffer)) {
		printf("%s", buffer);
	}
	
	printf("%s", buffer);
	
	return 0;
}

int fermeture(int fp) {
	return close(fp);
}
