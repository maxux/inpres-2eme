#ifndef __UI_H
	#define __UI_H
	
	#include <iostream>
	#include <cstring>
	#include <termios.h>
	
	#define FILENAME_LOGIN		"etc/Login.txt"
	
	#define ADMIN_DEFAULT_PASSWORD	"admin"
	
	#define USER_LEVEL_ADMIN	0
	#define USER_LEVEL_COLLECT	1
	#define USER_LEVEL_DESIGNER	2
	
	using namespace std;
	
	class UI {
		public:
			UI();
			~UI();
			
			int login();
			
			void disable_echo();
			void restore_echo();
			
			int check_login();
			
			inline int anti_flood(int value);
			
			void prepare();
			int start_events();
			
		private:
			tcflag_t c_lflag;
			string _login, _passwd;
			int _level;
	};
#endif
