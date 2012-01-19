#include "MyList.hxx"
using namespace std;

//Constructeurs
template<class T> MyList<T>::MyList()
{
	pList=NULL;
	nbElem=0;
}
template<class T> MyList<T>::~MyList()
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

template<class T>void MyList<T>::affiche(void)const
{
	cout << "Affichage de la liste." << endl;
	if(pList!=NULL)
	{
		noeud<T> *pCur=pList;
		while(pCur!=NULL)
		{
			cout << pCur->val << endl;
			pCur=pCur->ptSv;
		}
		cout << "Fin de la liste." << endl;
	}
	else
		cout << "Liste vide." << endl;
}

template<class T>int MyList<T>::size(void)const
{
	return nbElem;
}
template<class T>bool MyList<T>::vide(void)const
{
	if(pList==NULL)
		return true;
	else
		return false;
}

template<class T>void MyList<T>::ajoutePremier(T first)
{
	noeud<T> *pNew=new noeud<T>;

	pNew->val=first;	//Ajoute de la valeur passé en param.
	if(vide())			//On vérifie si la liste est vide
		pNew->ptSv=NULL;//Si oui, ptSv=NULL
	else
		pNew->ptSv=pList;//Sinon ptSv=pList
	pList=pNew;

	nbElem++;
}
template<class T>void MyList<T>::ajouteDernier(T last)
{
	/*if(nbElem==0)
	{
		ajoutePremier(last);
		return;
	}*/
	noeud<T> *pNew=new noeud<T>;
	noeud<T> *pLast;

	pNew->val=last;
	pNew->ptSv=NULL;

	pLast=pList;

	while(pLast->ptSv!=NULL)
		pLast=pLast->ptSv;

	pLast->ptSv=pNew;

	nbElem++;
}

template<class T>T MyList<T>::retirePremier(void)
{
	/*if(nbElem<=0)
	{
		cout << "La liste est vide." << endl;
		return -1;
	}*/

	noeud<T> *pLast=pList;
	T val=pList->val;

	pList=pList->ptSv;
	delete pLast;
	nbElem--;

	return val;
}

template<class T>T MyList<T>::retireDernier(void)
{
	T val;

	if(nbElem<=1)
	{
		val=retirePremier();
		return val;
	}

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

template<class T>int MyList<T>::save(std::fstream &f)
{
	//Nombre d'element pour la lecture
	f.write((char *)&nbElem, sizeof(nbElem));

	//Ecriture de la liste
	IterateurL<T> itL(*this); 
	if(!MyList::vide())
	{
		do{ // Federation Admin
			f.write((char*)&(&itL), sizeof(T));
		}while(++itL);
	}
	return 1;
}
template<class T>int MyList<T>::load(fstream &f)
{
	int nbElemTemp;
	//Nombre d'element pour la lecture
	f.read((char *)&nbElemTemp, sizeof(int));

	//Ecriture de la liste
	T temp;
	for(int i=0;i<nbElemTemp;i++)
	{
		f.read((char *)&temp, sizeof(T));
		ajoutePremier(temp);
	}

	return 1;
}

template class MyList<int>;
