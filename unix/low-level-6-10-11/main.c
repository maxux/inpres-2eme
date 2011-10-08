#include <stdio.h>
#include <stdlib.h>
#include "fichiers.h"

int main(void) {
	int fd;
	write_data_t data;
	
	fd = ouverture("/tmp/data-danielma");
	
	printf("Name: ");
	while(fgets(data.name, sizeof(data.name), stdin) == NULL);
	
	printf("Surname: ");
	while(fgets(data.surname, sizeof(data.surname), stdin) == NULL);

	if(ajout_data(fd, &data))
		exit(EXIT_FAILURE);
	
	if(lire_data(fd))
		exit(EXIT_FAILURE);
	
	fermeture(fd);
	
	return 0;
}
