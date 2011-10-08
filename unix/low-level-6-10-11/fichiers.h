#ifndef __FICHIER_H
	#define __FICHIER_H
	
	#include "main.h"
	
	int ouverture(const char *filename);
	int ajout_data(int fd, const write_data_t *data);
	int lire_data(int fd);
	int fermeture(int fp);

#endif
