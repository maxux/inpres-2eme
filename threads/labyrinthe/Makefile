CC=gcc

EXEC    = labyrinthe
CFLAGS  = -g -O2 -W -Wall
LDFLAGS = -lrt -lpthread -lSDL

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(PROGRAMS)
