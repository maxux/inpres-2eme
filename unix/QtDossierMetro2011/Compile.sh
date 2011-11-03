g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -c ./Ecran/fctTrace.cpp
g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -o Controle Controle.cpp fctTrace.o 
g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -o Chemin Chemin.cpp fctTrace.o   -lqt
g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -c FenetrePlanVille.cpp
g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -c moc_FenetrePlanVille.cpp
g++ -Wall -D SUN -I ./Ecran -I /usr/local/qt/include -I /usr/local/qt/mkspec/default -o QtDossierMetro2011 Main.cpp FenetrePlanVille.o moc_FenetrePlanVille.o fctTrace.o  -lqt -L/usr/local/qt/lib -L/usr/openwin/lib
