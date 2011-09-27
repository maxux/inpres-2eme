#ifndef __CARTE_H
	#define __CARTE_H
	
	#include <iostream>
	
	class Carte {
		private:
			char _name[30];
			char *_serie;
			int _id;			
		
		public:
			/* Construct */
			Carte(const char *name = "New", const char *serie = "New", int id = -1);
			Carte(Carte &original);
			
			/* Affecting */
			void setNumero(int id);
			void setNomCarte(const char *name);
			void setNomSerie(const char *serie);
			
			/* Return */
			char* getNomCarte();
			char* getNomSerie();
			int getNumero();
			
			/* Display */
			void Affiche() const;
			void Affiche(std::ostream &stream) const;
			
			/* Encoding */
			void Encode(std::istream &stream);
			
			/* Overload */
			friend std::ostream & operator << (std::ostream &flux, const Carte &carte);
			friend std::istream & operator >> (std::istream &flux, Carte &carte);
	};
#endif
