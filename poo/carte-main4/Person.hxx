#ifndef __PERSON_HEADER_H
	#define __PERSON_HEADER_H
	
	#include "Date.hxx"
	#include <iostream>
	
	using namespace std;

	class Person {
		public:
			Person();
			Person(Person &original);
			Person(const char *name, const char *surname, const char *born, const char *nation);
			virtual char * getIdentification();
			
			/* Display */
			void Affiche();
			
			/* Setters */
			void setNom(const char *name);
			void setPrenom(const char *name);
			void setDateNais(const char *name);
			void setNationalite(const char *name);
			
			/* Getters */
			const char * getNom();
			const char * getPrenom();
			const char * getDateNais();
			const char * getNationalite();
			
			/* Overload */
			friend std::ostream & operator << (ostream &flux, Person const &source);
			friend std::istream & operator >> (istream &flux, Person &source);
			
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;
			
			
		
		private:
			char _name[32];
			char _surname[32];
			char _nation[32];
			//Date _born;
			char _born[11];
	};
#endif
