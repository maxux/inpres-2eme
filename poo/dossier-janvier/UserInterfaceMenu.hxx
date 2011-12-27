#ifndef __UI_MENU_H
	#define __UI_MENU_H
	
	#include <iostream>
	#include <cstring>
	
	using namespace std;
	
	typedef UIMenu_item {
		string name;
		char key;
		
		struct UIMenu_item *next;
		
		
	} UIMenu_item;
	
	class UIMenu : public UI {
		public:
			UIMenu();
			~UIMenu();
			
		private:
			string _name;
			int _nbitem;
			UIMenu_item *_items;
			UIMenu_item *_last;
	};
#endif

