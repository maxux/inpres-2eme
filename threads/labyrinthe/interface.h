#ifndef __INTERFACE_H
	#define __INTERFACE_H

	int get_tab(position_t pos);
	int get_tab_nonblock(position_t pos);
	int set_tab(position_t pos, int value);
	int set_tab_nonblock(position_t pos, int value);
	
	position_t get_position_hero();
	position_t get_position_hero_nonblock();
	position_t set_position_hero(position_t pos);
	
	position_t get_destination_hero();
	position_t set_destination_hero(position_t pos);

	position_t mk_position_t(int L, int C);
	int compare_position(position_t a, position_t b);
	
	int get_nbcle();
	int set_nbcle(int value);
	
	position_t get_listetache(int indice);
	position_t get_listetache_nonblock(int indice);
	position_t set_listetache(position_t tache, int indice);
	
	int get_indiceinsertion();
	int set_indiceinsertion(int value);
	
	int get_indiceextraction();
	int get_indiceextraction_nonblock();
	int set_indiceextraction(int value);
	int set_indiceextraction_nonblock(int value);
	
	int get_heropix();
	int set_heropix(int value);
	
	
	void debug_tab();
#endif
