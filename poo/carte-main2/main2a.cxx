#include "CaractScore.hxx"
#include "Carte.hxx"
#include <iostream>
#include <fstream>
using namespace std;
#include <string.h>

void attente();
int main()
{
	cout<<"-------------Test de la classe CaractScore-----------------------"<<endl;
	cout<<"(1)Test constructeur par defaut + Affiche"<<endl;
	CaractScore c1;
	c1.Affiche();
	attente();
	cout<<"(2)Test des setters"<<endl;
	c1.setNomCaract("Amitie");
	c1.setScore(10);
	c1.Affiche();
	attente();
	cout<<"(3)Test du constructeur d'initialisation complet"<<endl;
	CaractScore c2("Audace",15);
	c2.Affiche();
	attente();
	cout<<"(4)Test du constructeur de copie"<<endl;
	CaractScore c3(c2);
	c3.Affiche();
	c3.setNomCaract("Autorite");
	c3.setScore(26);
	c3.Affiche();
	attente();
	cout<<"(6)Test d'allocation dynamique"<<endl;
	CaractScore * p3=new CaractScore();
	p3->setNomCaract("Qualite Star");
	p3->setScore(34);
	p3->Affiche();
	attente();
	cout<<"La Caracteristique de la carte est: "<<p3->getNomCaract()<<endl;
	cout<<"Le score associe a la caracteristique est: "<<p3->getScore()<<endl;
	delete p3;
	attente();
	cout<<"(7)Test des operateurs << et >>"<<endl;
	CaractScore c4;
	cout<<"Encodage de la caracteristique et du score d'une carte"<<endl;
	cin>>c4;
	cout<<"Affichage de la caracteristique et du score de la carte"<<endl;
	cout<<c4;
	attente();
	cout<<CaractScore::AMITIE<<endl;
	attente();
	cout<<CaractScore::AUTORITE<<endl;
	attente();
	CaractScore::BEST_QUALITE_STAR.Affiche();
	attente();
	CaractScore c5(CaractScore::BEST_AMITIE);
	c5.Affiche();
	return 0;
}

void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
