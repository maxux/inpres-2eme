#include"Date.h"
#include"Person.h"
#include"AuthorisedPerson.h"
#include"Collectionneur.h"
#include"ConcepteurAlbum.h"
void attente();
int main()
{

	cout<<"(1)Test classe Person"<<endl;
	Person p;
	cin>>p;
	cout<<p;
	attente();
	Person p1("Dupont","Marc","14/09/1975","belge");
	p1.Affiche();
	attente();
	p1=p;
	p1.Affiche();
	attente();
	Person p2("Durant","Jean","21/05/1978","italien");
	Person p3(p2);
	p3.Affiche();
	attente();
	cout<<endl<<endl;



	cout<<"(2)Test classe Collectionneur et des meth. virt. Affiche et getIdentification"<<endl;
	Collectionneur c;
	cout<<c;
	cin>>c;
	cout<<c;
	attente();
	Collectionneur c1("Dupont","Pierre","12/12/2000","belge","dudu",5);
	c1.Affiche();
	attente();
	c1=c;
	c1.Affiche();
	attente();
	Collectionneur c2("Durant","Gilles","03/10/1990","allemand","DUR",2);
	Collectionneur c3(c2);
	c3.Affiche();
	attente();
	c3.setNom("Legrand");
	Person *p4=&c3;
	p4->Affiche();
	attente();
	cout<<p4->getIdentification()<<endl;
	attente();
	Collectionneur *pc=new Collectionneur(c2);
	pc->Affiche();
	attente();
	cout<<pc->getIdentification()<<endl;
	delete pc;
	attente();
	Person *pp=new Collectionneur(c);
	pp->Affiche();
	attente();
	cout<<pp->getIdentification()<<endl;
	delete pp;
	attente();
	Person *p5=new Person();
	p5->setNom("Petit");
	p5->setPrenom("Eric");
	p5->setDateNais("5/5/2005");
	p5->setNationalite("hollandais");
	p5->Affiche();
	attente();
	cout<<p5->getIdentification()<<endl;
	delete p5;
	attente();

	cout<<" VECTEUR HETEROGENE  "<<endl;
	Person p6("Blaise","Luc","17/08/1975","belge");
	Person * vp[3]={&c1,&p6,&c2};
	int i;
	for(i=0;i<3;i++)
		vp[i]->Affiche();
	attente();
	cout<<endl<<endl;	


	cout<<"(3)Test classe ConcepteurAlbum et meth. virt. Affiche et getIdentification"<<endl;
	ConcepteurAlbum ca;
	cout<<ca;
	attente();
	ca.Affiche();
	attente();
	cout<<"  TEST DES SETTERS  "<<endl;
	ca.setNom("Legros");
	ca.setPrenom("Fabrice");
	ca.setDateNais("17/06/1968");
	ca.setNationalite("belge");
	ca.setLogin("Leg");
	ca.setNomSociete("Delhaize");
	cout<<ca;
	attente();
	ConcepteurAlbum ca1("Blaise","Jean","01/01/1950","espagnol","Blabla","Disney");
	ca1.Affiche();
	attente();
	ConcepteurAlbum ca2(ca1);
	cout<<ca2;
	attente();
	ConcepteurAlbum ca3;
	cin>>ca3;
	ca3.Affiche();
	attente();
	cout<<ca3.getIdentification()<<endl;
	attente();
	ca2=ca3;
	ca2.Affiche();
	attente();
	cout<<endl<<endl;
	Person * ptca=&ca3;
	ptca->Affiche();
	attente();
	cout<<ptca->getIdentification()<<endl;
	attente();
	

}

void attente()
{
	char t;
	cout<<"--->[RETURN] pour continuer<---";
	cin.get(t);
	cout<<endl;
}
