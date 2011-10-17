#include"CaractScore.hxx"
#include <stdlib.h>

void attente();
int main()
{
	cout<<"-------------Test de la classe CaractScore-----------------------"<<endl;
	CaractScore c,c1("Amitie",15),c2(CaractScore::BEST_AMITIE),c3(c1),
	total;
	int score;

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
	cout<<"(1)Test operateur affectation c=c1"<<endl;
	c=c1;
	c.Affiche();
	attente();
	cout<<"(2)Test operateur + pour augmenter le score c1=c1+3"<<endl;
	cout<<"c1"<<endl;
	c1.Affiche();
	c1=c1+3;
	c1.Affiche();
	attente();
	cout<<"(3)Test operateur - pour diminuer le score c3=c2-1"<<endl;
	cout<<"c2"<<endl;
	c2.Affiche();
	c3=c2-1;
	cout<<"c2"<<endl;
	c2.Affiche();
	cout<<"c3"<<endl;
	c3.Affiche();
	attente();
	cout<<"(4)Test operateur - pour calculer la difference de 2 scores score=c-c2"<<endl;
	cout<<"c"<<endl;
	c.Affiche();
	cout<<"c2"<<endl;
	c2.Affiche();
	score=c-c2;
	cout<<"difference de score = "<<score<<endl;
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
	if(c2<c3)
		cout<<"le score de c2 est le plus petit"<<endl;
	else
		cout<<"le score de c3 est plus petit ou egal au score de c2"<<endl;
	cout<<"c2"<<endl;
	c2.Affiche();
	cout<<"c3"<<endl;
	c3.Affiche();
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
