#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>


#ifndef DEF_STRUCTNOEUD
#define DEF_STRUCTNOEUD
	template <class T>
	struct noeud{
		T val;
		noeud<T> *ptSv;
	};
#endif
template <class T>class IterateurSl;

#ifndef DEF_MYSORTEDLIST
#define DEF_MYSORTEDLIST

	#define CROISSANT 0
	#define DECROISSANT 1

	template<class T> class MySortedList
	{
		protected:
			noeud<T> *pList;
			int nbElem, mode;

		public:
			MySortedList();
			~MySortedList();

			void affiche(void)const;
			//void setMode();
			void insere(T data);
			bool vide(void)const;
			int size(void)const;

			T retirePremier(void);
			T retireDernier(void);
			int retirerElement(T _elem);

			int save(std::fstream &f);
			int load(std::fstream &f);
	
			friend class IterateurSl<T>;
	};

#endif

#ifndef ITERATEUR_SL
#define ITERATEUR_SL

	template <class T>class IterateurSl
	{
		protected:
			MySortedList<T>& pObj;
			noeud<T> *pCur;
			int nbElem, pos;
		public:
			IterateurSl(MySortedList<T>& obj):pObj(obj), pCur(obj.pList), nbElem(obj.nbElem), pos(0){};

			void reset()
			{ 
				pCur=pObj.pList; 
				pos=0; 
				nbElem=pObj.nbElem;
			}
			int operator++()
			{
				if(pos<(nbElem-1) && pos!=nbElem)
				{
					pCur=pCur->ptSv;
					pos++;
					return 1;
				}
				else
					return 0;
			}
			operator T()const{return pCur->val;}
			T& operator&(){return pCur->val;}
	};

#endif
