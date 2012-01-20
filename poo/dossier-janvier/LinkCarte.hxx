#ifndef __LINK_CARTE_H
	#define __LINK_CARTE_H
	
	#include "MyContainers.hxx"
	#include "Album.hxx"
	#include "ConcepteurAlbum.hxx"
	#include "Collection.hxx"
	
	#define FILENAME_DESIGNER	"data/concepteurs.dat"
	
	using namespace std;
	
	class LinkCarte {
		public:
			MaListe <ConcepteurAlbum*> _concept;
			MaListe <ConcepteurAlbum*> :: iterator _cit;
			
			Album *_alb;
			
			Collection *_coll;
			
			string _login;
			
			LinkCarte();
			virtual ~LinkCarte();
			
			void setIteratorDesigner(string login);
			
			
		private:
			
	};
#endif

