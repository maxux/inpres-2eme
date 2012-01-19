#include <iostream>
#include <termios.h>
#include <string>

#include "UserInterface.hxx"
#include "UserInterfaceMenu.hxx"

UIMenu::UIMenu() {
	_nbitem = 0;
	_items = NULL;
	_last  = NULL;
}

UIMenu::~UIMenu() {
	UIMenu_item *current, *naow;
	
	if(_items != NULL) {
		_last   = NULL;
		current = _items;
		
		/* Cleaning list */
		while(current != NULL) {
			naow = current;
			current = current->next;
			
			delete naow;
		}
	}
}

void UIMenu::create(const char *name) {
	_name = name;
}

void UIMenu::append(const char *label, char key, int (*ptr)(LinkCarte *), LinkCarte *arg, bool endkey) {
	UIMenu_item *create;
	
	create = new UIMenu_item;
	
	/* Copy Data */
	create->name     = label;
	create->key      = key;
	create->pointer  = ptr;
	create->argument = arg;
	
	/* Init Pointer */
	create->next     = NULL;
	
	/* Building Links */
	if(_last != NULL) {
		_last->next = create;
		_last = create;
		
	} else {
		_items = create;
		_last  = create;
	}
	
	_nbitem++;
	
	if(endkey)
		_endkey = key;
}

int UIMenu::select(char key) {
	UIMenu_item *current;
	
	current = _items;
	
	while(current != NULL) {
		if(current->key == key) {
			if(current->pointer != NULL)
				return current->pointer(current->argument);
			else
				return 0;
		}
		
		current = current->next;
	}
	
	return 1;
}

int UIMenu::process() {
	UIMenu_item *current;
	size_t i;
	string choice;
	char key;
	
	current = _items;
	
	cout << " " << _name << endl;
	cout << " ";
	
	for(i = 0; i < _name.length(); i++)
		cout << "=";
	
	cout << endl << endl;
	
	while(current != NULL) {
		cout << " " << current->key << ") " << current->name << endl;
		current = current->next;
	}
	
	cout << endl;
	
	while(1) {
		cout << "# ";
		cin >> choice;
		
		key = choice.at(0);
		
		select(key);
		
		if(key == _endkey)
			break;
		
		cout << endl;
	}
	
	cout << endl;
	
	return 1;
}
