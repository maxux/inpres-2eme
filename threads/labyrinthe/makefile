.SILENT:

ASTAR=AStar
GRILLESDL=GrilleSDL

CC=g++ -D SUNRAY1 -I$(ASTAR) -I$(GRILLESDL) -I /opt/guengel/include/SDL
# SUNRAY1       pour la machine sunray1 de l'école
# VMWARESOLARIS pour la machine virtuelle solaris fournie en debut d'annee

ALL: Labyrinthe
OBJS = $(ASTAR)/AStar.o $(GRILLESDL)/GrilleSDL.o  
PROGRAMS = Labyrinthe

Labyrinthe:	Labyrinthe.c $(OBJS)
	echo Creation de Labyrinthe...
	$(CC) Labyrinthe.c -o Labyrinthe $(OBJS) -lrt -lpthread -lSDL

$(ASTAR)/AStar.o:	$(ASTAR)/AStar.c $(ASTAR)/AStar.h
		echo Creation de AStar.o ...
		$(CC) -c $(ASTAR)/AStar.c
		mv AStar.o $(ASTAR)

$(GRILLESDL)/GrilleSDL.o:	$(GRILLESDL)/GrilleSDL.c $(GRILLESDL)/GrilleSDL.h
		echo Creation de GrilleSDL.o ...
		$(CC) -c $(GRILLESDL)/GrilleSDL.c
		mv GrilleSDL.o $(GRILLESDL)

clean:
	@rm -f $(OBJS) core

clobber:	clean
	@rm -f tags $(PROGRAMS)
