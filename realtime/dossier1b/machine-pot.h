#ifndef __MACHINE_POT_H
#define __MACHINE_POT_H

	#define DEFAULT_MODEL		1
	#define SIMUL_POT		5
	
	#define MAX_GEN_POT_TIME	5

	typedef struct semap_t {
		sem_t *free;
		sem_t *used;

	} semap_t;
	
#endif
