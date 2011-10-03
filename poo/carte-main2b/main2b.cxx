#include "Carte.hxx"
#include "CaractScore.hxx"

void attente();
int main()
{

	cout<<"Test de la classe Carte avec une instance d'un objet de la classe CaractScore"<<endl;


	cout<<"(1)Test constructeur par defaut + Affiche"<<endl;
	Carte c1;
	c1.Affiche();
	attente();
	cout<<"(2)Test des setters"<<endl;
	c1.setNomCarte("Sally");
	c1.setNomSerie("Cars");
	c1.setNumero(141);
	c1.setCaractScore("Autorite",45);
	c1.Affiche();
	attente();
	CaractScore cs1("Audace",10);
	c1.setCaractScore(cs1);
	c1.Affiche();
	attente();
	cout<<"(3)Test du constructeur d'initialisation complet"<<endl;
	Carte c2("Bob","Monsters",47,"Amitie",24);
	c2.Affiche();
	attente();
	Carte c3("Violette","The incredibles",45,cs1);
	c3.Affiche();
	attente();
	cout<<"(5)Test du constructeur de copie"<<endl;
	Carte c4(c3);
	c4.Affiche();
	attente();
	cout<<"(6)Test d'allocation dynamique"<<endl;
	Carte * p2=new Carte();
	p2->setNomCarte("Woody");
	p2->setNomSerie("ToyStory");
	p2->setNumero(1);
	p2->setCaractScore("Autorite",56);
	p2->Affiche();
	attente();
	cout<<"Le nom de la carte est: "<<p2->getNomCarte()<<endl;
	cout<<"La nom de la serie est: "<<p2->getNomSerie()<<endl;
	cout<<"Le numero de la carte est: "<<p2->getNumero()<<endl;
	cout<<p2->getCaractScore();
	attente();
	p2->setCaractScore(cs1);
	p2->Affiche();
	delete p2;
	attente();
	cout<<"(7)Test des operateurs << et >>"<<endl;
	Carte c5;
	cout<<"Encodage des informations d'une carte"<<endl;
	cin>>c5;
	cout<<"Affichage des informations de la carte encodee"<<endl;
	cout<<c5;
	attente();
	Carte c6("Rex","ToyStory",6,CaractScore::BEST_QUALITE_STAR);
	c6.Affiche();
	attente();
	c6.setCaractScore(CaractScore::AMITIE,250);
	c6.Affiche();
	attente();
	cout<<c6.getCaractScore();
	exit(0);
}

void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
