#ifndef __AUTHORISED_HEADER_H
	#define __AUTHORISED_HEADER_H
	
	#include "Person.hxx"
	
	using namespace std;

	class AuthorisedPerson : public Person {
		public:
			AuthorisedPerson();
			AuthorisedPerson(const char *name, const char *surname, const char *born, const char *nation, const char *login);
			AuthorisedPerson(AuthorisedPerson &original);
			
			/* Setters */
			virtual void setLogin(const char *login);
			
			/* Getters */
			virtual const char * getLogin();
		
		private:
			char _login[32];
	};
#endif
