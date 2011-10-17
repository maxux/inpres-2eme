#include "Carte.hxx"
#include <stdlib.h>
void attente();
int main()
{
	cout<<"-------------Test de la classe Carte avec la surcharge des operateurs-----------------------"<<endl;
	
	Carte c, c1("Woody", "ToyStory",1, CaractScore::AUTORITE, 47), c2(c1),
	c3("BuzzLeclair", "ToyStory",4, CaractScore::BEST_QUALITE_STAR);
	int deltascore;

	cout<<"Affichage c"<<endl;
	c.Affiche();
	attente();
	cout<<"Affichage c1"<<endl;
	c1.Affiche();
	attente();
	cout<<"Affichage c2"<<endl;
	c2.Affiche();
	attente();
	cout<<"Affichage c3"<<endl;
	c3.Affiche();
	attente();
	cout<<"(1)Test operateur affectation c=c3"<<endl;
	c=c3;
	c.Affiche();
	attente();
	cout<<"(2)Test operateur + pour augmenter le score c1=c1+20"<<endl;
	cout<<"c1"<<endl;
	c1.Affiche();
	c1=c1+20;
	c1.Affiche();
	attente();
	cout<<"(3)Test operateur - pour diminuer le score c3=c2-10"<<endl;
	cout<<"c2"<<endl;
	c2.Affiche();
	c3=c2-10;
	cout<<"c2"<<endl;
	c2.Affiche();
	cout<<"c3"<<endl;
	c3.Affiche();
	attente();
	cout<<"(4)Test operateur - pour calculer la difference de 2 scores deltascore=c-c2 et deltascore=c1-c2"<<endl;
	cout<<"c"<<endl;
	c.Affiche();
	cout<<"c1"<<endl;
	c1.Affiche();
	cout<<"c2"<<endl;
	c2.Affiche();
	//verifier que caract de meme nom
	cout<<"c-c2"<<endl;
	if(c.IdentNomCaract(c2))
	{
		deltascore=c-c2;
		cout<<"difference de score = "<<deltascore<<endl;
	}
	else
		cout<<"Nom de caracteristique different"<<endl;
	cout<<"c1-c2"<<endl;
	if(c1.IdentNomCaract(c2))
	{
		deltascore=c1-c2;
		cout<<"difference de score = "<<deltascore<<endl;
	}
	else
		cout<<"Nom de caracteristique different"<<endl;
	attente();
	cout<<"(5)Test operateur <<      cout<<c; cout<<c2;"<<endl;
	cout<<"c"<<endl;
	cout<<c;
	cout<<"c2"<<endl;
	cout<<c2;
	attente();
	cout<<"(6)Test operateur de comparaison if(c2<c3) afficher message"<<endl;
	cout<<"c2"<<endl;
	c2.Affiche();
	cout<<"c3"<<endl;
	c3.Affiche();
	//verifier que caract de meme nom
	if(c2.IdentNomCaract(c3))
	{
		if(c2<c3)
			cout<<"le score de c2 est plus petit que le score de c3"<<endl;
		else
			cout<<"le score de c3 est plus petit ou egal au score de c2"<<endl;
		cout<<"c2"<<endl;
		c2.Affiche();
		cout<<"c3"<<endl;
		c3.Affiche();
	}
	else
		cout<<"Nom de caracteristique different"<<endl;
	attente();
	cout<<"(7)Test pre-incrementation cout<<++c"<<endl;
	cout<<"c"<<endl;
	cout<<c;
	cout<<"++ c"<<endl;
	cout<<++c;
	cout<<"c"<<endl;
	cout<<c;
	attente();
	cout<<"(9)Test post-incrementation cout<<c2++"<<endl;
	cout<<"c2"<<endl;
	cout<<c2;
	cout<<"c2++"<<endl;
	cout<<c2++;
	cout<<"c2"<<endl;
	cout<<c2;

	exit(0);
}

void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
