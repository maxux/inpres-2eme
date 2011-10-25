CC      = gcc
CFLAGS  = -W -Wall -O2 -pipe -g -D DEBUG
LDFLAGS = 
EXEC    = Applic

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -fv *.o

mrproper: clean
	rm -fv $(EXEC)
	rm -fv Index Data.dat
