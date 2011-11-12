#ifndef __C_VECTEUR_H
	#define __C_VECTEUR_H
	
	#include <iostream>
	#include <cstring>
	using namespace std;
	
	template <class T> class Vecteur {
		public:
			Vecteur(unsigned int n = 1) {
				_size = n;
				
				/* Init Vector */
				_v = new T[n];
				
				/* Init Present Vector */
				_present = new char[n];
				memset(_present, 0, sizeof(char) * n);
				
				/* Allocating flags */
				_nb    = 0;
			}
			
			Vecteur(const Vecteur &original) {
				
			}
			
			virtual ~Vecteur() {
				delete _v;
			}
			
			void Affiche() {
				unsigned int i;
	
				if(empty())
					return;
					
				for(i = 0; i < _size; i++)
					if(Present(i))
						cout << "Element " << i << ": " << *(_v + i) << endl;
			}
			
			bool IndiceValide(unsigned int indice) {
				return (indice < _size);
			}
			
			bool Present(unsigned int indice) {
				if(IndiceValide(indice))
					return *(_present + indice);
				else
					return 0;
			}
			
			bool empty() {
				// cout << "DEBUG: (empty) " << (_nb == 0) << endl;
				return (_nb == 0);
			}
			
			bool full() {
				// cout << "DEBUG: (full) " << (_nb == _size) << endl;
				return (_nb == _size);
			}
			
			/* Overload */
			int operator + (T id) {
				unsigned int i;
				
				if(full())
					return 0;
				
				/* Serching first free */
				for(i = 0; i < _size; i++)
					if(!*(_present + i))
						break;
					
				// cout << "DEBUG: Inserting [] at indice [" << i << "]" << endl;
					
				/* Inserting element */
				*(_v + i) = id;
				
				/* Marking it present */
				*(_present + i) = 1;
				
				_nb++;
				
				return 1;
			}
			
			int operator + (Vecteur &rval) {
				cout << "adding2..." << endl;
				return 0;
			}
			
			int operator - (T id) {
				unsigned int i, this_nb;
				
				/* Saving _nb to compare it after */
				this_nb = _nb;
				
				/* Reading the vector. Stopping if he is empty */
				for(i = 0; (i < _size) && !(empty()); i++) {
					/* cout << "CMP: " << (*(_v + i) == id) << endl;
					cout << "PREZ" << (bool) *(_present + i) << endl; */
					
					if(*(_present + i) && (*(_v + i) == id)) {
						// cout << "Unset indice [" << i << "]" << endl;
						*(_present + i) = 0;
						_nb--;
					}
				}
				
				return !(this_nb == _nb);
			}
			
			T operator [] (int i) {
				return *(_v + i);
			}
		
		private:
			T *_v;
			char *_present;
			
			unsigned int _size;
			unsigned int _nb;
	};
#endif
