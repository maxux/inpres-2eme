#include "MySortedList.hxx"
using namespace std;

template<class T> MySortedList<T>::MySortedList()
{
	pList=NULL;
	nbElem=0;
	mode=CROISSANT;
}

template<class T> MySortedList<T>::~MySortedList()
{
	noeud<T> *pCur, *pPrec;
	pCur=pList;
	while(pCur!=NULL)
	{
		pPrec=pCur;
		pCur=pCur->ptSv;
		delete pPrec;
	}
}

template<class T>void MySortedList<T>::affiche(void)const
{
	cout << "Affichage de la liste triee." << endl;
	if(pList!=NULL)
	{
		noeud<T> *pCur=pList;
		while(pCur!=NULL)
		{
			cout << endl;
			pCur->val.affiche();
			cout << endl;
			pCur=pCur->ptSv;
		}
		cout << "Fin de la liste." << endl;
	}
	else
		cout << "Liste vide." << endl;
}

template<class T>void MySortedList<T>::insere(T data)
{
	noeud<T> *pCur, *pPrecedent;
	noeud<T> *pNew=new noeud<T>;
	pNew->val=data;
	pNew->ptSv=NULL;

	if(pList==NULL) //Insertion au début quand liste vide
	{
		//cout << "Liste vide" << endl;
		pList=pNew;
	}
	else
	{
		if(pNew->val<=pList->val) //Insertion au début
		{
			//cout << "Liste début" << endl;
			pNew->ptSv=pList;
			pList=pNew;
		}
		else
		{
			pCur=pList;
			while(pCur!=NULL && data>pCur->val)
			{
				pPrecedent=pCur;
				pCur=pCur->ptSv;
			}
			if(pCur==NULL && data>pPrecedent->val) //Insertion en fin de liste
			{
				//cout << "Liste fin" << endl;
				pPrecedent->ptSv=pNew;
			}
			else //Insertion au milieu de la liste
			{
				//cout << "Liste milieu" << endl;
				pPrecedent->ptSv=pNew;
				pNew->ptSv=pCur;
			}
		}
	}
	nbElem++;
}

template<class T>T MySortedList<T>::retirePremier(void)
{
	//if(nbElem<=0)
	//{
	//	cout << "La liste triee est vide." << endl;
	//	return -1;
	//}

	noeud<T> *pLast=pList;
	T val=pList->val;

	pList=pList->ptSv;
	delete pLast;
	nbElem--;
	
	return val;
}
template<class T>T MySortedList<T>::retireDernier(void)
{
	T val;

	//if(nbElem<=1)
	//{
	//	val=retirePremier();
	//	return val;
	//}

	noeud<T> *pLast=pList;
	noeud<T> *pEnd;

	while(pLast->ptSv!=NULL)
	{
		pEnd=pLast;
		pLast=pLast->ptSv;
	}
	pEnd->ptSv=NULL;
	val=pLast->val;
	delete pLast;
	nbElem--;

	return val;
}
template<class T>int MySortedList<T>::retirerElement(T _elem)
{
	if(pList!=NULL)
	{
		noeud<T> *pCur=pList, *pPrecedent=pList;

		if(pCur->ptSv==NULL)
		{
			if(_elem == pCur->val)
			{
				nbElem--;
				pList=NULL;
				//delete pCur;
				return 1;
			}
		}
		else
		{
			while(pCur->ptSv!=NULL)
			{
				pCur=pCur->ptSv;
				if(_elem == pCur->val)
				{
					nbElem--;
					pPrecedent->ptSv=pCur->ptSv;
					//delete pCur;
					return 1;
				}
				pPrecedent=pCur;
			}
		}
	}
	return 0;
}

template<class T>int MySortedList<T>::size(void)const
{
	return nbElem;
}
template<class T>bool MySortedList<T>::vide(void)const
{
	if(pList==NULL)
		return true;
	else
		return false;
}

template<class T>int MySortedList<T>::save(fstream &f)
{
	//Nombre d'element pour la lecture
	f.write((char *)&nbElem, sizeof(int));

	//Ecriture de la liste
	IterateurSl<T> itSL(*this); 
	if(!MySortedList::vide())
	{
		do{ // Federation Admin
			f.write((char*)&(&itSL), sizeof(T));
		}while(++itSL);
	}
	return 1;
}

template<class T>int MySortedList<T>::load(fstream &f)
{
	int nbElemTemp;
	//Nombre d'element pour la lecture
	f.read((char *)&nbElemTemp, sizeof(int));

	//Ecriture de la liste
	T temp;
	for(int i=0;i<nbElemTemp;i++)
	{
		f.read((char *)&temp, sizeof(T));
		insere(temp);
	}

	return 1;
}
