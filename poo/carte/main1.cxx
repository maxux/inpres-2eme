#include "carte.hxx"
#include <iostream>
#include <fstream>
using namespace std;
#include <string.h>

void attente();
int main()
{

	cout<<"(1)Test constructeur par defaut + Affiche"<<endl;
	Carte c1;
	c1.Affiche();
	attente();
	cout<<"(2)Test des setters"<<endl;
	c1.setNomCarte("Sally");
	c1.setNomSerie("Cars");
	c1.setNumero(141);
	c1.Affiche();
	attente();
	cout<<"(3)Test du constructeur d'initialisation complet"<<endl;
	Carte c2("Bob","Monsters",47);
	c2.Affiche();
	attente();
	cout<<"(4)Test du constructeur d'initialisation partiel"<<endl;
	Carte c3("Boule");
	c3.Affiche();
	attente();
	cout<<"(5)Test du constructeur de copie"<<endl;
	Carte c4(c3);
	c4.setNomSerie("Nemo");
	c4.setNumero(77);
	c4.Affiche();
	attente();
	cout<<"(6)Test d'allocation dynamique"<<endl;
	Carte * p1=new Carte();
	p1->setNomCarte("Woody");
	p1->setNomSerie("ToyStory");
	p1->setNumero(1);
	p1->Affiche();
	attente();
	cout<<"Le nom de la carte est: "<<p1->getNomCarte()<<endl;
	cout<<"La nom de la serie est: "<<p1->getNomSerie()<<endl;
	cout<<"Le numero de la carte est: "<<p1->getNumero()<<endl;
	delete p1;
	attente();
	cout<<"(7)Test des operateurs << et >>"<<endl;
	Carte c5;
	cout<<"Encodage des caracteristiques d'une carte"<<endl;
	cin>>c5;
	cout<<"Affichage des caracteristiques de la carte encodee"<<endl;
	cout<<c5;
	return 0;
}

void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
