#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Applic.h"
#include "interface.h"
#include "interface-link.h"

menu_t * menu_create(char *title, int exit_id) {
	menu_t *menu;
	
	/* Init Menu */
	menu = (menu_t *) malloc(sizeof(menu_t));
	if(menu == NULL)
		return menu;
	
	/* Init Title */
	menu->name = (char *) malloc(sizeof(char) * strlen(title) + 1);
	if(menu->name == NULL) {
		free(menu);
		return NULL;
	}
	
	strcpy(menu->name, title);

	/* Init nodes */
	menu->count_entry = 0;
	
	menu->nodes = NULL;
	menu->last  = NULL;
	
	menu->exit_id = exit_id;
	
	return menu;
}

void menu_append(menu_t *menu, char *entry, int id, int (*pointer)(FICHIERVAR*), FICHIERVAR *argument) {
	menu_node_t *node;
	
	node = (menu_node_t *) malloc(sizeof(menu_node_t));
	if(node == NULL)
		return;
	
	node->text = (char *) malloc(sizeof(char) * strlen(entry) + 1);
	if(node->text == NULL) {
		free(node);
		return;
	}
	
	/* Setting up entry */
	strcpy(node->text, entry);
	node->id = id;
	
	/* Updating link */
	node->next = NULL;
	
	/* Updating root link */
	if(menu->nodes == NULL) {
		menu->nodes = node;
		
	} else menu->last->next = node;
	
	menu->last = node;
	menu->count_entry++;
	
	node->pointer = pointer;
	node->argument = argument;
}

int menu_process(menu_t *menu) {
	menu_node_t *temp, *selected = NULL;
	int i, len, entry;
	char buffer[32];
	
	
	len = strlen(menu->name) + 2;
	
	printf("\n " COLOR_BLUE "%s" COLOR_NONE "\n", menu->name);
	for(i = 0; i < len; i++)
		printf("-");
	
	printf("\n\n");
	
	temp = menu->nodes;
	
	while(temp != NULL) {
		printf(" %d) %s\n", temp->id, temp->text);
		temp = temp->next;
	}
	
	printf("\n");
	
	while(selected == NULL) {
		printf("> ");
		
		if(fgets(buffer, sizeof(buffer), stdin) == NULL)
			continue;
		
		entry = atoi(buffer);
		
		if(entry != 0) {
			temp = menu->nodes;
	
			while(temp != NULL) {
				if(temp->id == entry) {
					selected = temp;
					break;
				}
				
				temp = temp->next;
			}
		}
	}
	
	debug("[+] Interface: Selected entry: %d\n", entry);
	
	if(entry == menu->exit_id)
		return 0;
	
	if(selected->pointer != NULL) {
		debug("[+] Interface: Calling function pointer...\n");
		selected->pointer(selected->argument);
	}
	
	return 1;
}

void menu_free(menu_t *menu) {
	menu_node_t *temp, *me;
	
	temp = menu->nodes;
	
	while(temp != NULL) {
		if(temp->text != NULL)
			free(temp->text);
		
		/* Deleting current instance */
		me = temp;
		temp = temp->next;
		
		free(me);
	}
	
	if(menu->name != NULL)
		free(menu->name);
		
	free(menu);
}
