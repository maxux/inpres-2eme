#ifndef __UI_H
	#define __UI_H
	
	#include <iostream>
	#include <string>
	#include <termios.h>
	
	#include "LinkCarte.hxx"
	
	#define FILENAME_LOGIN		"etc/Login.txt"
	#define FILENAME_LOGIN_TEMP	"/tmp/login.rewrite.temp"
	#define PATH_DATA		"data/"
	
	#define ADMIN_DEFAULT_PASSWORD	"admin"
	
	#define USER_LEVEL_ADMIN	0
	#define USER_LEVEL_COLLECT	1
	#define USER_LEVEL_DESIGNER	2
	
	using namespace std;
	
	class UI {
		public:
			UI();
			virtual ~UI();
			
			int login();
			
			void disable_echo();
			void restore_echo();
			
			int check_login(string login);
			int check_login_exists(string login);
			
			inline int anti_flood(int value);
			
			int user_level(string username);
			
			void prepare();
			int start_events(LinkCarte *origin);
			
		private:
			tcflag_t c_lflag;
			string _login, _passwd;
			int _level;
	};
#endif
