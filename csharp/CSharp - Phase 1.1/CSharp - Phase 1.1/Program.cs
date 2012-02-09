///////////////////////////////////////////////
// UN SEUL DEFINE NON COMMENTE A LA FOIS !!! //
///////////////////////////////////////////////

// #define TEST1
// #define TEST2
// #define TEST3
// #define TEST4
// #define TEST5
#define TEST6


using System;
using System.Collections.Generic;
using System.Text;
// using MyClassLib;

namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            #region TEST1 : Test des constructeurs et propriétés
#if TEST1
            Activite a1 = new Activite();
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls",new DateTime(2012,1,23,8,20,0),null,"Seraing","Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés",new DateTime(2012,6,30,22,0,0),new DateTime(2012,7,1,8,0,0),"Le Carré","Je passerai, mais incognito car je serai pas moflé ^^");

           

            Console.WriteLine("a1:" + a1);
            Console.WriteLine("\na2:" + a2);
            Console.WriteLine("\na3:" + a3);
            Console.WriteLine("\na4:" + a4);

            Console.WriteLine("Appuyez sur une touche pour modifier a4");
            Console.ReadKey();

            a4.SetActivites(new DateTime(2012, 9, 15, 22, 0, 0), new DateTime(2012, 9, 16, 8, 0, 0), "Le Carré", "Je passerai, mais pour feter mon entrée en 3ème ^^");
            Console.WriteLine("a4:\t" + a4);

            Console.WriteLine("Appuyez sur une touche pour afficher le resumé des activités");
            Console.ReadKey();

            Console.WriteLine("a1:" + a1.ToString("Resume",null));
            Console.WriteLine("\na2:" + a2.ToString("Resume", null));
            Console.WriteLine("\na3:" + a3.ToString("Resume", null));
            Console.WriteLine("\na4:" + a4.ToString("Resume", null));

            Console.WriteLine("Appuyez sur une touche pour afficher le detail des activités");
            Console.ReadKey();

            Console.WriteLine("a1:" + a1.ToString("Detail", null));
            Console.WriteLine("a2:" + a2.ToString("Detail", null));
            Console.WriteLine("a3:" + a3.ToString("Detail", null));
            Console.WriteLine("a4:" + a4.ToString("Detail", null));

            

            Console.WriteLine("Appuyez sur une touche pour terminer");
            Console.ReadKey();
#endif
            #endregion

            #region TEST2 : Test de l'égalité d'activités à l'aide de la méthode "Equals"
#if TEST2
            Activite a1 = new Activite("Le bal des moflés");
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls",new DateTime(2012,1,23,8,20,0),null,"Seraing","Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés",new DateTime(2012,6,30,22,0,0),new DateTime(2012,7,1,8,0,0),"Le Carré","Je passerai, mais incognito car je serai pas moflé ^^");

            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A2 : " + a2);

            if (a1.Equals(a2))
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");            
            }
            else
            {
                Console.WriteLine("OK : " + a1.Nom + " est différent de " + a2.Nom);
            }

            Console.WriteLine();

            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A3 : " + a3);

            if (a1.Equals(a3))
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");
            }
            else
            {
                Console.WriteLine("OK : " + a1.Nom + " est différent de " + a3.Nom);
            }

            Console.WriteLine();


            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A4 : " + a4);

            if (a1.Equals(a4))
            {
                Console.WriteLine("Parfait : " + a1.Nom + " == " + a4.Nom);
            }
            else
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");
            }

            Console.ReadKey();

        
#endif
            #endregion

            #region TEST3 : Test de l'égalité d'activités à l'aide de l'opérateur ==
#if TEST3
            Activite a1 = new Activite("Le bal des moflés");
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls", new DateTime(2012, 1, 23, 8, 20, 0), null, "Seraing", "Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés", new DateTime(2012, 6, 30, 22, 0, 0), new DateTime(2012, 7, 1, 8, 0, 0), "Le Carré", "Je passerai, mais incognito car je serai pas moflé ^^");

            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A2 : " + a2);

            if (a1 == a2)
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");
            }
            else
            {
                Console.WriteLine("OK : " + a1.Nom + " est différent de " + a2.Nom);
            }

            Console.WriteLine();

            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A3 : " + a3);

            if (a1 == a3)
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");
            }
            else
            {
                Console.WriteLine("OK : " + a1.Nom + " est différent de " + a3.Nom);
            }

            Console.WriteLine();


            Console.WriteLine("Test de l'égalité entre : ");
            Console.WriteLine("A1 : " + a1);
            Console.WriteLine("et");
            Console.WriteLine("A4 : " + a4);

            if (a1 == a4)
            {
                Console.WriteLine("Parfait : " + a1.Nom + " == " + a4.Nom);
            }
            else
            {
                Console.WriteLine("Oooops ! Something is wrong :( ... Try again !!!");
            }

            Console.ReadKey();

#endif
            #endregion

            #region TEST4 : Test présence d'un objet dans une liste
#if TEST4
            List<Activite> ListeActivites = new List<Activite>();

            Activite a1 = new Activite("Le bal des moflés");
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls", new DateTime(2012, 1, 23, 8, 20, 0), null, "Seraing", "Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés", new DateTime(2012, 6, 30, 22, 0, 0), new DateTime(2012, 7, 1, 8, 0, 0), "Le Carré", "Je passerai, mais incognito car je serai pas moflé ^^");


            ListeActivites.Add(a1);

            if (!ListeActivites.Contains(a2))
            {
                ListeActivites.Add(a2);
            }

            if (!ListeActivites.Contains(a3))
            {
                ListeActivites.Add(a3);
            }

            if (!ListeActivites.Contains(a4))
            {
                ListeActivites.Add(a4);
                Console.WriteLine("Ooops ! On ne devrait pas pouvoir ajouter une pièce ayant la meme clé qu'une pièce dejà présente dans la liste");
            }

            foreach (Activite a in ListeActivites)
            {
                Console.WriteLine(a.ToString("Resume",null));
            }

            if (ListeActivites.Count == 3)
            {
                Console.WriteLine("Ok la liste contient bien 3 activités");
            }
            else
            {
                Console.WriteLine("Ooops !!! La liste devrait contenir 3 activités");
            }
            
      
            Console.ReadKey();
#endif
            #endregion

            #region TEST5: Comparaison d'activités via le tri dans une liste
#if TEST5

            List<Activite> ListeActivites = new List<Activite>();

            Activite a1 = new Activite("La nuit des publivores");
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls", new DateTime(2012, 1, 23, 8, 20, 0), null, "Seraing", "Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés", new DateTime(2012, 6, 30, 22, 0, 0), new DateTime(2012, 7, 1, 8, 0, 0), "Le Carré", "Je passerai, mais incognito car je serai pas moflé ^^");

            ListeActivites.Add(a2);
            ListeActivites.Add(a4);
            ListeActivites.Add(a3);
            ListeActivites.Add(a1);


            Console.WriteLine("Affichage de la liste non triée");

            foreach (Activite a in ListeActivites)
            {
                Console.WriteLine(a.ToString("Resume", null));
            }


            try
            {
                Console.WriteLine("Tentative de tri");
                ListeActivites.Sort();

                Console.WriteLine("Affichage de la liste triée");
                
                foreach (Activite a in ListeActivites)
                {
                    Console.WriteLine(a.ToString("Resume", null));
                }
            }
            catch (Exception exc)
            {
                Console.WriteLine("Ooops ! Le tri n'a pu s'effectuer !!!");
                
            }

            try
            {
                Console.WriteLine("Tentative de tri sur la date début via un délégué");
                Comparison<Activite> compActivites = new Comparison<Activite>(Activite.ComparisonDateDebut);
                ListeActivites.Sort(compActivites);

                Console.WriteLine("Affichage de la liste triée sur la date de début");

                foreach (Activite a in ListeActivites)
                {
                    Console.WriteLine(a.ToString("Resume", null));
                }
            }
            catch (Exception exc)
            {
                Console.WriteLine("Ooops ! Le tri n'a pu s'effectuer !!!");
            }

            // Ajouter la fonctionalité de tri d'activités par date de début décroissante
      
            Console.ReadKey();
#endif
            #endregion

            #region TEST6 : Une classe dédicacée à la comparaison
#if TEST6
            List<Activite> ListeActivites = new List<Activite>();

            Activite a1 = new Activite("La nuit des publivores");
            Activite a2 = new Activite("Le step en 10 leçons");
            Activite a3 = new Activite("C# pour les nuls", new DateTime(2012, 1, 23, 8, 20, 0), null, "Amsterdam", "Yeah ! je vais enfin coder dans mon futur langage preferé :p");
            Activite a4 = new Activite("Le bal des moflés", new DateTime(2012, 6, 30, 22, 0, 0), new DateTime(2012, 7, 1, 8, 0, 0), "Le Carré", "Je passerai, mais incognito car je serai pas moflé ^^");

            a1.Lieu = "Zanzibar";
            a2.Lieu = "Ibiza";

            ListeActivites.Add(a2);
            ListeActivites.Add(a4);
            ListeActivites.Add(a3);
            ListeActivites.Add(a1);


            Console.WriteLine("Affichage de la liste non triée");

            foreach (Activite a in ListeActivites)
            {
                Console.WriteLine(a.ToString("Resume", null));
            }


            try
            {
                Console.WriteLine("Tentative de tri");
                ListeActivites.Sort();

                Console.WriteLine("Affichage de la liste triée");
                
                foreach (Activite a in ListeActivites)
                {
                    Console.WriteLine(a.ToString("Resume", null));
                }
            }
            catch (Exception exc)
            {
                Console.WriteLine("Ooops ! Le tri n'a pu s'effectuer !!!");
                
            }

            try
            {
                Console.WriteLine("Tentative de tri sur le lieu via un objet 'comparer'");
                MyActiviteComparer myComparer = new MyActiviteComparer();
                ListeActivites.Sort(myComparer);

                Console.WriteLine("Affichage de la liste triée sur le lieu");

                foreach (Activite a in ListeActivites)
                {
                    Console.WriteLine(a.ToString("Resume", null));
                }
            }
            catch (Exception exc)
            {
                Console.WriteLine("Ooops ! Le tri n'a pu s'effectuer !!!");
            }

        
            Console.ReadKey();

#endif
            #endregion

        }
    }
}
