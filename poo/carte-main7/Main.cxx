
/* La partie 3. Test de la classe FichierCircuit n'a pas encore été adaptée avec le downcasting, il faut dans les méthodes de la classe FichierCircuit (save, list, ...) aller supprimer les lignes ds lesquelles on vient positionner la variable bool disque*/

#include"FichierLog.h"
#include"Carte.h"
#include"FichierCarte.h"
void attente();
int main()
{

cout<<"----------------------------------Les flux-------------------------------------"<<endl;

cout<<"1. Test de la classe FichierLog------------------------------------------------"<<endl;
FichierLog fl("FanaDeCollections.log");
fl<<"Login de Woody(ToyStory)";
fl<<"Collectionneur Tintin possede 3 collections";
fl<<"Essai termine"<<"fin ...";
system("cat ./FanaDeCollections.log"); //Affiche le contenu du fichier
attente();


cout<<"---2.Ecriture de deux cartes ds un fichier-------------------------------"<<endl;
cout<<"Sauvegarde des cartes et relecture"<<endl;
Carte c1("Bob","Monsters",47,"Amitie",24);
CaractScore cs1("Audace",10);
Carte c2("Sally","Cars",141,cs1);
cout<<c1;
cout<<c2;
attente();

fstream f;
f.open("Essai.txt",ios::out|ios::app);
f<<c1;
f<<c2;
cout<<"Ecriture des deux cartes sur le disque"<<endl;
f.close();
cout<<"Fermeture fichier"<<endl;
attente();

f.open("Essai.txt",ios::in);
if(!f.rdbuf()->is_open())
	{
		cout<<"Erreur d'ouverture du fichier"<<endl;
		
	}
cout<<"Ouverture fichier"<<endl;
attente();
Carte c;
cout<<"Lecture des deux premieres cartes sans la fct eof"<<endl;
f>>c;
cout<<c<<endl;
f>>c;
cout<<c<<endl;
f.close();
attente();


cout<<"Lecture des cartes avec la fct eof"<<endl;
f.open("Essai.txt",ios::in);
f>>c;
while(!f.eof())
{
	cout<<c<<endl;
	f>>c;
}
f.clear();	
f.close();
attente();


cout<<"---3. Test de la classe FichierCarte--------------------------------------"<<endl;
cout<<"---Sauvegarde de cartes et relecture---------------------------------------"<<endl;
cout<<"---Ecriture des deux cartes c1 et c2 dans le fichier Cartes.txt--------"<<endl;
FichierCarte fic("Cartes.txt");
fic.save(c1);
fic.save(c2);
attente();

cout<<"Lecture du fichier et affichage des cartes lues dans le fichier-------------"<<endl;
fic.list();
attente();

cout<<"Recherche d'une carte a partir du nom de la carte-------------------------"<<endl;
bool ret;
ret=fic.load(c,"Sally");
if(ret==true)
{
	cout<<"carte trouvee"<<endl;
	cout<<"affichage de la carte: "<<endl;
	cout<<c<<endl;
}
else
	cout<<"carte non trouvee"<<endl;
attente();
ret=fic.load(c,"Bobette");
if(ret==true)
{
	cout<<"carte trouvee"<<endl;
	cout<<"affichage de la carte: "<<endl;
	cout<<c<<endl;
}
else
	cout<<"carte non trouvee"<<endl;



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
