DEFINES="-DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_GUI_LIB -DQT_SHARED"
CFLAGS="-pipe -Wall -W -D_REENTRANT ${DEFINES}"
INCPATH="-I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -IEcran -I."
LFLAGS="-Wl,-O1 -Wl,-rpath,/usr/lib64/qt4"
LIBS="-L/usr/lib64/qt4 -lQtGui -L/usr/lib64 -L/usr/lib64/qt4 -L/usr/X11R6/lib -lQtCore -lgthread-2.0 -lrt -lglib-2.0 -lpthread"

g++ $CFLAGS $INCPATH -c ./Ecran/fctTrace.cpp
g++ $CFLAGS $INCPATH -o Controle Controle.cpp fctTrace.o 
g++ $LFLAGS $LIBS $INCPATH -o Chemin Chemin.cpp fctTrace.o
g++ $CFLAGS $INCPATH -c FenetrePlanVille.cpp
g++ $CFLAGS $INCPATH -c moc_FenetrePlanVille.cpp
g++ $LFLAGS $LIBS $INCPATH -o QtDossierMetro2011 Main.cpp FenetrePlanVille.o moc_FenetrePlanVille.o fctTrace.o
