#ifndef __AUTHORISED_HEADER_H
	#define __AUTHORISED_HEADER_H
	
	#include "Person.hxx"
	
	using namespace std;

	class AuthorisedPerson : public Person {
		public:
			AuthorisedPerson();
			AuthorisedPerson(AuthorisedPerson &original);
			virtual char * getIdentification();
			
			/* Setters */
			virtual void setLogin(char *login);
		
		private:
			char _login[32];
	};
#endif
