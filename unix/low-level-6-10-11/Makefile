CC      = x86_64-pc-linux-gnu-gcc
CFLAGS  = -W -Wall -O2 -pipe -g
LDFLAGS = 
EXEC    = main

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
