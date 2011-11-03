CC      = sparc-sun-solaris2.10-gcc
CFLAGS  = -W -Wall -O2 -pipe -g -D DEBUG -D COLOR
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
	rm -fv $(EXEC) Index Data.dat
