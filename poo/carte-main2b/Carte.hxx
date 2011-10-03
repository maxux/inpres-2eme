#ifndef __CARTE_H
	#define __CARTE_H
	
	#include <iostream>
	#include "CaractScore.hxx"
	
	class Carte {
		private:
			char _name[30];
			char *_serie;
			int _id;
			CaractScore *caract;
		
		public:
			/* Construct */
			Carte(const char *name = "New", const char *serie = "New", int id = -1, const char *caract_name = "New", int caract_score = 0);
			Carte(const char *name = "New", const char *serie = "New", int id = -1, CaractScore &original);
			Carte();
			Carte(Carte &original);
			
			/* Affecting */
			void setNumero(int id);
			void setNomCarte(const char *name);
			void setNomSerie(const char *serie);
			
			void setCaractScore(const char *name, int score);
			void setCaractScore(CaractScore &original);
			
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
			friend std::ostream & operator << (std::ostream &flux, Carte const &carte);
			friend std::istream & operator >> (std::istream &flux, Carte &carte);
	};
#endif
