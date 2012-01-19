#ifndef __UI_MENU_H
	#define __UI_MENU_H
	
	#include <iostream>
	#include <cstring>
	
	#include "LinkCarte.hxx"
	
	using namespace std;
	
	typedef struct UIMenu_item {
		string name;
		char key;
		
		int (*pointer)(LinkCarte*);
		LinkCarte *argument;
		
		struct UIMenu_item *next;
		
	} UIMenu_item;
	
	class UIMenu : public UI {
		public:
			UIMenu();
			virtual ~UIMenu();
			
			void create(const char *name);
			void append(const char *label, char key, int (*ptr)(LinkCarte *), LinkCarte *arg, bool endkey = 0);
			
			int process();
			int select(char key);
			
		private:
			string _name;
			int _nbitem;
			UIMenu_item *_items;
			UIMenu_item *_last;
			char _endkey;
	};
#endif

