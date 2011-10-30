#ifndef __INTERFACE_H
	#define __INTERFACE_H
	
	#include "AccFichVar.h"
	
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
		int (*pointer)(FICHIERVAR *);
		FICHIERVAR *argument;
		
		/* Link Element */
		struct menu_node_t *next;
		
	} menu_node_t;
	
	menu_t * menu_create(char *title, int exit_id);
	void menu_append(menu_t *menu, char *entry, int id, int (*pointer)(FICHIERVAR*), FICHIERVAR *argument);
	int menu_process(menu_t *menu);
	void menu_free(menu_t *menu);
	
	#ifdef COLOR
		#define COLOR_BLUE	"\033[1;34m"
		#define COLOR_GREY	"\033[1;30m"
		#define COLOR_GREEN	"\033[1;32m"
		#define COLOR_RED	"\033[1;31m"
		#define COLOR_NONE	"\033[0m"
	#else
		#define COLOR_BLUE	""
		#define COLOR_GREY	""
		#define COLOR_GREEN	""
		#define COLOR_RED	""
		#define COLOR_NONE	""
	#endif
#endif
