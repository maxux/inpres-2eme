#include "MyVector.hxx"
using namespace std;

//Constructeurs
template<class T> MyVector<T>::MyVector(int n)
{
	data=new T[n];
	vecPlace=new int[n];
	nbrElem=n;

	for(int i=0;i<n;i++)
		*(vecPlace+i)=0;
}

template<class T> MyVector<T>::~MyVector()
{
	delete vecPlace;
}

//Setter
template<class T>void MyVector<T>::setElement(int indice, const T &valeur)
{
	*(vecPlace+indice)=1;
	*(data+indice)=valeur;
}

template<class T>bool MyVector<T>::setElement(const T &valeur)
{
	for(int i=0;i<nbrElem;i++)
	{
		if(*(vecPlace+i)==0)
		{
			*(vecPlace+i)=1;
			*(data+i)=valeur;
			return true;
		}
	}
	cout << "Pas de place dans le vecteur" << endl;
	return false;
}

//Getter
template<class T>T MyVector<T>::getElement(int indice)const
{
	return *(data+indice);
}

template<class T>int MyVector<T>::getNbElem()const
{
	return nbrElem;
}
template<class T>int MyVector<T>::getNbElemPos()
{
	int nb=0;
	for(int i=0;i<nbrElem;i++)
	{
		if(*(vecPlace+i)==1)
			nb++;
	}
	return nb;
}
//Affichage
template<class T>void MyVector<T>::affiche(void)const
{
	cout << "Affichage du vecteur" << endl;
	for(int i=0;i<nbrElem;i++)
	{
		if(*(vecPlace+i)==1)
			cout << "La valeur a la position " << i << " est: " << endl << *(data+i) << endl << endl; 
	}
}

//Autres
template<class T>bool MyVector<T>::indiceValide(int indice)const
{
	if((nbrElem-indice)>=0)
		return true;
	else
		return false;
}

template<class T>bool MyVector<T>::estOccupe(int indice)const
{
	if(*(vecPlace+indice)==0)
		return false;
	else
		return true;
}

template<class T>T MyVector<T>::retireElement(int indice)
{
	*(vecPlace+indice)=0;
	return *(data+indice);
}

template<class T>  void MyVector<T>::operator+(const T &o)
{
	setElement(o);
}

template class MyVector<int>;
template class MyVector<char *>;
