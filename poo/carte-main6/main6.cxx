#include "Date.hxx"
#include "CaractScore.hxx"
#include "Carte.hxx"
#include "Person.hxx"
#include "AuthorisedPerson.hxx"
#include "Collectionneur.hxx"
#include "ConcepteurAlbum.hxx"
#include "ExceptionBase.hxx"
#include "Vecteur.hxx"
#include <stdlib.h>
void attente();
int main()
{

	cout<<"VECTEUR TEMPLATE"<<endl<<endl;

	/* cout<<"1. TEST AVEC UN VECTEUR D'ENTIERS"<<endl;
	Vecteur<int>ve1(5);
	
	cout<<"---------------Affichage des elements du vecteur----------------------------"<<endl;
	ve1.Affiche();
	attente();
	cout<<"---------------Ajout d'elements dans le vecteur et affichage du vecteur-----"<<endl;
	ve1+5;
	ve1+4;
	ve1.Affiche();
	attente();
	cout<<"---------------Affichage d'un element du vecteur-----------------------------"<<endl;	
	cout<<"Affichage de l'element d'indice 1--------------------------------------------"<<endl;
	if(ve1.IndiceValide(1) && ve1.Present(1))
		cout<<ve1[1]<<endl;
	else
		cout<<"affichage impossible"<<endl;
	attente();
	cout<<"Affichage de l'element d'indice 2--------------------------------------------"<<endl;
	if(ve1.IndiceValide(2) && ve1.Present(2))
		cout<<ve1[2]<<endl;
	else
		cout<<"affichage impossible"<<endl;
	attente();
	cout<<"Affichage de l'element d'indice -1-------------------------------------------"<<endl;
	if(ve1.IndiceValide(-1) && ve1.Present(-1))
		cout<<ve1[-1]<<endl;
	else
		cout<<"affichage impossible"<<endl;
	attente();
	cout<<"---------------Tests pour savoir si le vecteur est vide ou plein-------------"<<endl;	
	cout<<"vecteur vide ? vrai=1,faux=0"<<endl;
	cout<<ve1.empty()<<endl;
	cout<<"vecteur plein ? vrai=1,faux=0"<<endl;
	cout<<ve1.full()<<endl;
	attente();
	cout<<"---------------Suppression d'elements et tests pour savoir si le vecteur est vide ou-plein-----"<<endl;	
	cout<<"suppresion de l'entier 5 avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<ve1-5<<endl;
	attente();
	cout<<"suppresion de l'entier 10 avec affichade de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<ve1-10<<endl;
	attente();
	cout<<"vecteur vide ?"<<endl;
	cout<<ve1.empty()<<endl;
	cout<<"vecteur plein ?"<<endl;
	cout<<ve1.full()<<endl;
	attente();
	cout<<"suppresion de l'entier 4 avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<ve1-4<<endl;
	attente();

	cout<<"vecteur vide ?"<<endl;
	cout<<ve1.empty()<<endl;
	cout<<"vecteur plein ?"<<endl;
	cout<<ve1.full()<<endl;
	attente();
	cout<<endl; */

	cout<<"2. TEST AVEC UN VECTEUR de CARTE"<<endl<<endl;
	Vecteur<Carte>ve2(3);
	Carte c1,c2,c3,c4;
	cout<<"vecteur vide ? vrai=1,faux=0"<<endl;
	cout<<ve2.empty()<<endl;
	cout<<"vecteur plein ? vrai=1,faux=0"<<endl;
	cout<<ve2.full()<<endl;
	attente();
	cout<<"entrer une Carte1"<<endl;
	cin>>c1;
	cout<<"Ajout de Carte1 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve2+c1<<endl;
	cout<<"entrer une Carte2"<<endl;
	cin>>c2;
	cout<<"Ajout de Carte2 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve2+c2<<endl;
	cout<<"entrer une Carte3"<<endl;
	cin>>c3;
	cout<<"Ajout de Carte3 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve2+c3<<endl;
	cout<<"entrer une Carte4"<<endl;
	cin>>c4;
	cout<<"Ajout de Carte4 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve2+c4<<endl;
	attente();
	attente();
	cout<<"vecteur vide ?"<<endl;
	cout<<ve2.empty()<<endl;
	cout<<"vecteur plein ?"<<endl;
	cout<<ve2.full()<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur-----------------------------"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Carte qu'on supprime\n"<<c1<<endl;
	cout<<ve2-c1<<endl;
	attente();
	cout<<"Suite a cette suppression, affichage du vecteur Presence en utilisant la meth. Present"<<endl;
	cout<<ve2.Present(0)<<endl;
	cout<<ve2.Present(1)<<endl;
	cout<<ve2.Present(2)<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur-----------------------------"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Carte qu'on supprime\n"<<c4<<endl;
	cout<<ve2-c4<<endl;
	attente();
	cout<<"Affichage des elements du vecteur"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Carte qu'on ajoute\n"<<c4<<endl;
	cout<<"Ajout de la Carte c4 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve2+c4<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur---------------------------"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Affichage d'un element du vecteur------------------------------------------"<<endl;	
	cout<<"Affichage de l'element d'indice 2------------------------------------------"<<endl;
	if(ve2.IndiceValide(2) && ve2.Present(2))
		cout<<ve2[2]<<endl;
	else
		cout<<"affichage impossible"<<endl;
	attente();
	cout<<"---------------Affichage des element du vecteur----------------------------"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Carte qu'on supprime\n"<<c2<<endl;
	cout<<ve2-c2<<endl;
	attente();
	cout<<"--------------Affichage des element du vecteur-----------------------------"<<endl;
	ve2.Affiche();
	attente();
	cout<<"Affichage de l'element d'indice 1------------------------------------------"<<endl;
	if(ve2.IndiceValide(1) && ve2.Present(1))
		cout<<ve2[1]<<endl;
	else
		cout<<"affichage impossible"<<endl;

	attente();

	cout<<"3. TEST AVEC UN VECTEUR de COLLECTIONNEUR"<<endl<<endl;
	Vecteur<Collectionneur>ve3(3);
	Collectionneur col1,col2,col3,col4;
	cout<<"vecteur vide ? vrai=1, faux=0"<<endl;
	cout<<ve3.empty()<<endl;
	cout<<"vecteur plein ? vrai=1, faux=0"<<endl;
	cout<<ve3.full()<<endl;
	attente();
	cout<<"entrer un Collectionneur1"<<endl;
	cin>>col1;
	cout<<"Ajout du Collectionneur1 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve3+col1<<endl;
	cout<<"entrer un Collectionneur2"<<endl;
	cin>>col2;
	cout<<"Ajout du Collectionneur2 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve3+col2<<endl;
	cout<<"entrer un Collectionneur3"<<endl;
	cin>>col3;
	cout<<"Ajout du Collectionneur3 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve3+col3<<endl;
	cout<<"entrer un Collectionneur4"<<endl;
	cin>>col4;
	cout<<"Ajout du Collectionneur4 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve3+col4<<endl;
	attente();
	attente();
	cout<<"vecteur vide ?"<<endl;
	cout<<ve3.empty()<<endl;
	cout<<"vecteur plein ?"<<endl;
	cout<<ve3.full()<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur----------------------------"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Collectionneur qu'on supprime\n"<<col1<<endl;
	cout<<ve3-col1<<endl;
	attente();
	cout<<"Suite a cette suppression, affichage du vecteur Presence un utilisant la meth. Present"<<endl;
	cout<<ve3.Present(0)<<endl;
	cout<<ve3.Present(1)<<endl;
	cout<<ve3.Present(2)<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur-----------------------------"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Collectionneur qu'on supprime\n"<<col4<<endl;
	cout<<ve3-col4<<endl;
	attente();
	cout<<"Affichage des elements du vecteur"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Collectionneur qu'on ajoute\n"<<col4<<endl;
	cout<<"Ajout du Collectionneur c4 et affichage de 0 si l'ajout ne s'est pas fait sinon 1:  "<<ve3+col4<<endl;
	attente();
	cout<<"---------------Affichage des elements du vecteur------------------------------"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Affichage d'un element du vecteur---------------------------------------------"<<endl;	
	cout<<"Affichage de l'element d'indice 2---------------------------------------------"<<endl;
	if(ve3.IndiceValide(2) && ve3.Present(2))
		cout<<ve3[2]<<endl;
	else
		cout<<"affichage impossible"<<endl;
	attente();
	cout<<"---------------Affichage des element du vecteur-------------------------------"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Suppression d'un element du vecteur avec affichage de 0 si suppression non faite et 1 dans le cas contraire"<<endl;
	cout<<"Collectionneur qu'on supprime\n"<<col2<<endl;
	cout<<ve3-col2<<endl;
	attente();
	cout<<"--------------Affichage des element du vecteur--------------------------------"<<endl;
	ve3.Affiche();
	attente();
	cout<<"Affichage de l'element d'indice 1---------------------------------------------"<<endl;
	if(ve3.IndiceValide(1) && ve3.Present(1))
		cout<<ve3[1]<<endl;
	else
		cout<<"affichage impossible"<<endl;

	attente();

	exit(0);
}


void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
