CC      = x86_64-pc-linux-gnu-g++
DEFINES = -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS  = -W -Wall -O2 -pipe -g -D DEBUG -D_REENTRANT -I/usr/share/qt4/mkspecs/linux-g++ -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/Qt3Support -I/usr/include/qt4
LDFLAGS = -Wl,-O1 -Wl,-rpath,/usr/lib64/qt4 -L/usr/lib64/qt4 -lQt3Support -L/usr/lib64 -L/usr/lib64/qt4 -lQtSql -L/usr/lib64/mysql -lQtXml -L/usr/X11R6/lib -lQtNetwork -lQtGui -lQtCore -lgthread-2.0 -lrt -lglib-2.0 -lpthread
EXEC    = Chemin

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.cpp %.hxx
	$(CC) $(DEFINES) -o $@ -c $< $(CFLAGS)

clean:
	rm -fv *.o

mrproper: clean
	rm -fv $(EXEC)
