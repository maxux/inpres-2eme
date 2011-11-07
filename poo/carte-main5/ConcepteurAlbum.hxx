#ifndef __COLLECTIONNEUR_ALBUM_HEADER_H
	#define __COLLECTIONNEUR_ALBUM_HEADER_H
	
	#include "AuthorisedPerson.hxx"
	#include <iostream>
	
	using namespace std;

	class ConcepteurAlbum : public AuthorisedPerson {
		public:
			ConcepteurAlbum();
			ConcepteurAlbum(ConcepteurAlbum &original);
			ConcepteurAlbum(const char *name, const char *surname, const char *born, const char *nation, const char *login, const char *societe_name);
			virtual ~ConcepteurAlbum();
			virtual char * getIdentification();
			
			/* Display */
			void Affiche();
			
			/* Setters */
			void setNomSociete(const char *name);
			
			/* Getters */
			const char * getNomSociete();
			
			/* Overload */
			friend std::ostream & operator << (ostream &flux, ConcepteurAlbum const &source);
			friend std::istream & operator >> (istream &flux, ConcepteurAlbum &source);
			
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;
		
		private:
			char _inc_name[32];
			char *_id;
	};
#endif
