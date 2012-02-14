#ifndef __MACHINE_POT_H
#define __MACHINE_POT_H

	#define DEFAULT_MODEL		1
	#define SIMUL_POT		5
	
	#define MAX_GEN_POT_TIME	5
	
	#define FILL_IN_TIME		1.23
	
	typedef struct semap_global_t {
		sem_t total;
		sem_t tapis;
		sem_t spouts[];
		
	} semap_global_t;
	
#endif
