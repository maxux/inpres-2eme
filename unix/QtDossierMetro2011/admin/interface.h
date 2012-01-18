#ifndef __INTERFACE_H
	#define __INTERFACE_H
	
	#include "debug.h"
	
	typedef struct menu_t {
		char *name;
		int count_entry;
		int exit_id;
		
		struct menu_node_t *nodes;
		struct menu_node_t *last;
		
	} menu_t;
	
	typedef struct menu_node_t {
		char *text;
		char id;
		
		/* Function Pointer */
		int (*pointer)(void);
		void *argument;
		
		/* Link Element */
		struct menu_node_t *next;
		
	} menu_node_t;
	
	menu_t * menu_create(char *title, int exit_id);
	void menu_append(menu_t *menu, char *entry, int id, int (*pointer)(void), void *argument);
	int menu_process(menu_t *menu);
	void menu_free(menu_t *menu);
#endif
