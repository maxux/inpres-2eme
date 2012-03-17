#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "interface.h"

int get_tab(position_t pos) {
	int value = -1;
	
	// printf("[ ] Requesting get tab\n");
	
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return -1;
	}
	
	pthread_mutex_lock(&mutexTab);
	value = tab[pos.L][pos.C];
	pthread_mutex_unlock(&mutexTab);
	
	// printf("[ ] Requesting get tab: ok\n");
	
	return value;
}

int get_tab_nonblock(position_t pos) {
	int value = -1;
	
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return -1;
	}
	
	value = tab[pos.L][pos.C];
	
	return value;
}

int set_tab(position_t pos, int value) {
	// printf("[ ] Requesting set tab\n");
	
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return -1;
	}
	
	pthread_mutex_lock(&mutexTab);
	tab[pos.L][pos.C] = value;
	pthread_mutex_unlock(&mutexTab);
	
	// printf("[ ] Requesting set tab: done\n");
	
	return value;
}

int set_tab_nonblock(position_t pos, int value) {
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return -1;
	}
	
	tab[pos.L][pos.C] = value;
	
	return value;
}


position_t get_position_hero() {
	position_t value;
	
	// printf("[ ] Requesting get position\n");
	pthread_mutex_lock(&mutexPosition);
	value = position_hero;
	pthread_mutex_unlock(&mutexPosition);
	
	// printf("[ ] Requesting get position: done\n");
	
	return value;
}

position_t set_position_hero(position_t pos) {
	// printf("[ ] Requesting set position\n");
	
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return pos;
	}
	
	pthread_mutex_lock(&mutexPosition);
	position_hero = pos;
	pthread_mutex_unlock(&mutexPosition);
	
	// printf("[ ] Requesting set position: done\n");
	
	return pos;
}


position_t get_destination_hero() {
	position_t value;
	
	// printf("[ ] Requesting get destination\n");
	
	pthread_mutex_lock(&mutexDestination);
	value = destination_hero;
	pthread_mutex_unlock(&mutexDestination);
	
	// printf("[ ] Requesting get destination: done\n");
	
	return value;
}

position_t set_destination_hero(position_t pos) {	
	// printf("[ ] Requesting set destination\n");
	
	if(pos.L > NB_LIGNES || pos.C > NB_COLONNES) {
		fprintf(stderr, "[-] Overflow request\n");
		return pos;
	}
	
	pthread_mutex_lock(&mutexDestination);
	destination_hero = pos;
	pthread_mutex_unlock(&mutexDestination);
	
	// printf("[ ] Requesting set destination: done\n");
	
	return pos;
}

position_t mk_position_t(int L, int C) {
	position_t value;
	
	value.L = L;
	value.C = C;
	
	return value;
}

int compare_position(position_t a, position_t b) {
	return (a.L == b.L && a.C == b.C);
}

int get_nbcle() {
	int value;
	
	pthread_mutex_lock(&mutexNbCles);
	value = nbCles;
	pthread_mutex_unlock(&mutexNbCles);
	
	return value;
}

int set_nbcle(int value) {
	if(value < 0)
		value = 0;
		
	pthread_mutex_lock(&mutexNbCles);
	nbCles = value;
	pthread_mutex_unlock(&mutexNbCles);
	
	return value;
}

position_t get_listetache(int indice) {
	position_t tache = {0, 0};
	
	if(indice > 9) {
		fprintf(stderr, "[-] ListeTache: Overflow request\n");
		return tache;
	}
	
	pthread_mutex_lock(&mutexListeTaches);
	tache = ListeTaches[indice];
	pthread_mutex_unlock(&mutexListeTaches);
	
	return tache;
}

position_t get_listetache_nonblock(int indice) {
	position_t tache = {-1, -1};
	
	if(indice > 9) {
		fprintf(stderr, "[-] ListeTache: Overflow request\n");
		return tache;
	}
	
	tache = ListeTaches[indice];
	
	return tache;
}

position_t set_listetache(position_t tache, int indice) {
	if(indice > 9) {
		fprintf(stderr, "[-] ListeTache: Overflow request\n");
		return tache;
	}
	
	pthread_mutex_lock(&mutexListeTaches);
	ListeTaches[indice] = tache;
	pthread_mutex_unlock(&mutexListeTaches);
	
	return tache;
}

int get_indiceinsertion() {
	int value;
	
	pthread_mutex_lock(&mutexIndiceInsertion);
	value = indiceInsertion;
	pthread_mutex_unlock(&mutexIndiceInsertion);
	
	return value;
}

int set_indiceinsertion(int value) {
	value = (value > 9) ? 0 : value;
	
	pthread_mutex_lock(&mutexIndiceInsertion);
	indiceInsertion = value;
	pthread_mutex_unlock(&mutexIndiceInsertion);
	
	return value;
}

int get_indiceextraction() {
	int value;
	
	pthread_mutex_lock(&mutexIndiceExtraction);
	value = indiceExtraction;
	pthread_mutex_unlock(&mutexIndiceExtraction);
	
	return value;
}

int get_indiceextraction_nonblock() {
	int value;
	
	value = indiceExtraction;
	
	return value;
}

int set_indiceextraction(int value) {
	value = (value > 9) ? 0 : value;
	
	pthread_mutex_lock(&mutexIndiceExtraction);
	indiceExtraction = value;
	pthread_mutex_unlock(&mutexIndiceExtraction);
	
	return value;
}

int set_indiceextraction_nonblock(int value) {
	value = (value > 9) ? 0 : value;
	
	indiceExtraction = value;
	
	return value;
}

int get_heropix() {
	int value;
	
	pthread_mutex_lock(&mutexHeroPix);
	value = heroPix;
	pthread_mutex_unlock(&mutexHeroPix);
	
	return value;
}

int set_heropix(int value) {
	pthread_mutex_lock(&mutexHeroPix);
	heroPix = value;
	pthread_mutex_unlock(&mutexHeroPix);
	
	return value;
}
