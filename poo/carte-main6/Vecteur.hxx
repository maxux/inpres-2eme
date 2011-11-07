#ifndef __C_VECTEUR_H
	#define __C_VECTEUR_H
	
	#include <iostream>
	using namespace std;
	
	template <class T> class Vecteur {
		public:
			Vecteur(unsigned int n = 1) {
				_size = n;
				_v = new T[n];
				
				_nb    = 0;
				_empty = 1;
				_full  = 0;
			}
			
			Vecteur(const Vecteur &original) {
				
			}
			
			virtual ~Vecteur() {
				delete _v;
			}
			
			void Affiche() {
				int i;
	
				if(_empty)
					return;
					
				for(i = 0; i < _nb; i++)
					cout << "Element " << i << ": " << _v[i] << endl;
			}
			
			bool IndiceValide(unsigned int indice) {
				return true;
			}
			
			bool Present(unsigned int indice) {
				return true;
			}
			
			bool empty() {
				cout << "QUERYYYYYYYYYYYYY..." << endl;
				return _empty;
			}
			
			bool full() {
				return _full;
			}
			
			/* Overload */
			int operator + (int il) {
				cout << "adding..." << endl;
			}
			
			int operator + (Vecteur &rval) {
				cout << "adding2..." << endl;
			}
			
			int operator - (int i) {
				
			}
			
			int operator [] (int i) {
				return *(_v + i);
			}
		
		private:
			T *_v;
			unsigned int _size;
			unsigned int _nb;
			bool _empty;
			bool _full;
	};
#endif
