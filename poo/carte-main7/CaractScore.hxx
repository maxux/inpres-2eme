#ifndef __CARACT_HEADER_H
	#define __CARACT_HEADER_H
	
	#include <iostream>
	#include "ExceptionBase.hxx"
	
	using namespace std;

	class CaractScore {
		public:
			CaractScore(const char *name = "<unknown>", int score = 0);
			// virtual ~CaractScore();
			void __construct(const char *name, int score);

			static const char AMITIE[];
			static const char AUDACE[];
			static const char AUTORITE[];
			static const char QUALITE_STAR[];

			static const CaractScore BEST_AMITIE;
			static CaractScore BEST_QUALITE_STAR;
			
			void Affiche() const;

			void setNomCaract(const char *name);
			void setScore(int score);

			char * getNomCaract();
			int getScore();
			
			/* Overload Methods */
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;

			/* Overload */
			friend std::ostream & operator << (ostream &flux, CaractScore const &caract);
			friend std::istream & operator >> (istream &flux, CaractScore &caract);
			
			CaractScore & operator = (CaractScore &rval);
			
			CaractScore & operator + (int value);
			CaractScore & operator - (int value);
			int operator - (CaractScore &rval);
			
			bool operator < (CaractScore &rval);

			CaractScore & operator ++ ();
			CaractScore operator ++ (int);

		private:
			int _score;
			char _name[16];
	};
	
	class InvalidScoreException : public ExceptionBase {
		public:
			InvalidScoreException();
			InvalidScoreException(const char *message);
			InvalidScoreException(const InvalidScoreException &original);
			~InvalidScoreException();
	};
	
	class InvalidCaracteristiqueCarteException : public ExceptionBase {
		public:
			InvalidCaracteristiqueCarteException();
			InvalidCaracteristiqueCarteException(const char *message);
			InvalidCaracteristiqueCarteException(const InvalidCaracteristiqueCarteException &original);
			~InvalidCaracteristiqueCarteException();
	};
#endif
