CC=sparc-sun-solaris2.10-gcc
CFLAGS=-W -Wall -O3 -pipe -g
LDFLAGS=
EXEC=mysum

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

mysum: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -v *.o

mrproper: clean
	rm -v $(EXEC)

