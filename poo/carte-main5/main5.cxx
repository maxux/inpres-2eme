#include "Carte.hxx"
#include "CaractScore.hxx"
#include "ExceptionBase.hxx"
#include <stdlib.h>

void attente();
int main()
{
	
	int i;

	cout<<"(1)Test de la classe ExceptionBase"<<endl;

	cout<<"entrez 0 pour generer l'erreur ou une valeur entiere differente de 0 dans le cas contraire"<<endl;
	cout<<"entrer votre valeur"<<endl;
	cin>>i;
	try
	{
		if(i==0)
		{
			ExceptionBase e("test de la classe ExceptionBase");
			throw e;
		}
		cout<<"i="<<i<<endl;
	}
	catch(ExceptionBase ex)
	{
	
		cout<<ex.getMessage()<<endl;	
	} 
	attente();

	cout<<"(2)Test de la classe InvalidScoreException"<<endl;

	cout<<"entrez 0 pour generer l'erreur ou une valeur entiere differente de 0 dans le cas contraire"<<endl;
	cout<<"entrer votre valeur"<<endl;
	cin>>i;
	try
	{
		if(i==0)
		{
		
			InvalidScoreException is("test de la classe InvalidScoreException");
			throw is;
		}	
		cout<<"i="<<i<<endl;
	}
	catch(InvalidScoreException is )
	{
	
		cout<<is.getMessage()<<endl;	
	} 	
	attente();


	cout<<"(3)Test de la classe CaractScore pouvant lancer une InvalidScoreException"<<endl;

	cout<<"Test du constructeur d'initialisation complet avec score<=0"<<endl;
	CaractScore cs1("Autorite",-3);
	cout<<cs1;
	attente();

	cout<<"Test des setters avec score<=0"<<endl;
	CaractScore cs2;
	cs2.setNomCaract("Amitie");
	cs2.setScore(-5);
	cout<<cs2;

	cout<<"Test des operateurs >> et << "<<endl;
	CaractScore cs3;
	cout<<"Encodage de la caracteristique et du score d'une carte"<<endl;
	cin>>cs3;
	cout<<"Affichage de la caracteristique et du score de la carte"<<endl;
	cout<<cs3;
	attente();
	attente();

	cout<<"Test operateur - pour diminuer le score cs5=cs4-10"<<endl;
	CaractScore cs4("Audace",3);
	CaractScore cs5;
	cout<<"cs4"<<endl;
	cs4.Affiche();
	cs5=cs4-10;
	cout<<"cs4"<<endl;
	cs4.Affiche();
	cout<<"cs5"<<endl;
	cs5.Affiche();
	attente();
	cout<<"Test operateur - pour diminuer le score cs5=cs4-2"<<endl;
	cout<<"cs4"<<endl;
	cs4.Affiche();
	cs5=cs4-2;
	cout<<"cs4"<<endl;
	cs4.Affiche();
	cout<<"cs5"<<endl;
	cs5.Affiche();
	attente();

	cout<<"(4)Test de la classe Carte pouvant lancer une InvalidScoreException"<<endl;

	Carte c1("Bob","Monsters",47,"Amitie",-5);
	cout<<c1;
	attente();
	Carte c2("Violette","The incredibles",45,cs1);
	cout<<c2;
	attente();

	c2.setCaractScore("Audace",-2);
	cout<<c2;
	attente();

	Carte c3;
	cin>>c3;
	cout<<c3;
	attente();

	Carte c4("Rex","ToyStory",6,"Audace",5);
	Carte c5;
	cout<<"c4"<<endl;
	c4.Affiche();
	attente();
	c5=c4-10;
	cout<<"c4"<<endl;
	c4.Affiche();
	attente();
	cout<<"c5"<<endl;
	c5.Affiche();
	attente();

	cout<<"(5)Test de la classe Carte pouvant lancer une InvalidCaracteristiqueCarteException"<<endl;
	cout<<"Comparaison des scores de deux cartes"<<endl;
	
	Carte c6("Bob","Monsters",47,"Amitie",12);
	cout<<"c4"<<endl;
	c4.Affiche();
	cout<<"c6"<<endl;
	c6.Affiche();
	attente();
	
	try
	{
		if(c4.IdentNomCaract(c6))
		{
			InvalidCaracteristiqueCarteException ic("pour comparer le score de 2 cartes, la caracteristique des 2 cartes doit etre identique");
			throw ic;
		}
	
		if(c4<c6)
			cout<<"le score de c4 est plus petit que le score de c6"<<endl;
		else
			cout<<"le score de c4 est plus grand ou egal au score de c6"<<endl;
	}
	catch(InvalidCaracteristiqueCarteException ic)
	{
		cout<<ic.getMessage()<<endl;
	}
	attente();

	cout<<"Difference des scores de deux cartes"<<endl;
	int deltascore;
	try
	{
		if(c4.IdentNomCaract(c6))
		{
			InvalidCaracteristiqueCarteException ic("pour calculer la difference des scores de 2 cartes, la caracteristique des 2 cartes doit etre identique");
			throw ic;
		}
		deltascore=c4-c6;
		cout<<"difference de scores (c4-c6) = "<<deltascore<<endl;
	}
	catch(InvalidCaracteristiqueCarteException ic)
	{
		cout<<ic.getMessage()<<endl;
	}
	
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
