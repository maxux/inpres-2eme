#ifndef __COLLECTIONNEUR_HEADER_H
	#define __COLLECTIONNEUR_HEADER_H
	
	#include "AuthorisedPerson.hxx"
	#include <iostream>
	
	using namespace std;

	class Collectionneur : public AuthorisedPerson {
		public:
			Collectionneur();
			Collectionneur(Collectionneur &original);
			Collectionneur(const char *name, const char *surname, const char *born, const char *nation, const char *login, int nb_collection);
			virtual char * getIdentification();
			
			/* Display */
			void Affiche();
			
			/* Setters */
			void setNomCollection(const char *name);
			void setNbCollection(int collection);
			
			/* Getters */
			char * getNomCollection();
			int getNbCollection();
			
			/* Overload */
			friend std::ostream & operator << (ostream &flux, Collectionneur const &source);
			friend std::istream & operator >> (istream &flux, Collectionneur &source);
			
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;
		
		private:
			char _name[32];
			int _nb_collection;
	};
#endif
