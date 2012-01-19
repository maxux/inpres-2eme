#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>


template <class T>class IterateurVec;

#ifndef DEF_MYVECTOR
#define DEF_MYVECTOR
	template<class T> class MyVector
	{
		protected:
			T *data;
			int *vecPlace;
			int nbrElem;
			int nbElemPos;
		public:
			MyVector(int n=10);
			~MyVector();

			void setElement(int indice, const T &valeur);
			bool setElement(const T &valeur);

			T getElement(int indice)const;
			int getNbElem()const;
			int getNbElemPos();

			void affiche(void)const;

			bool indiceValide(int indice)const;
			bool estOccupe(int indice)const;
			T retireElement(int indice);

			void operator+(const T &o);
			friend class IterateurVec<T>;
	};
#endif

#ifndef ITERATEUR_VEC
#define ITERATEUR_VEC

	template <class T>class IterateurVec
	{
		protected:
			MyVector<T>& pObj;
			T* pData;
			int nbElem, pos;
		public:
			IterateurVec(MyVector<T>& obj):pObj(obj), pData(obj.data), nbElem(obj.getNbElemPos()), pos(0) {};

			void reset()
			{ 
				pData=pObj.data; 
				nbElem=pObj.getNbElemPos();
				pos=0;
			}


			int operator++()
			{
				if(pos<nbElem)
				{
					pData++;
					pos++;
					return 1;
				}
				else
					return 0;
			}
			operator T()const{return *pData;}
			T& operator&(){return *pData;}
	};

#endif

