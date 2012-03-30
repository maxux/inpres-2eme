CC      = gcc
DEFINES = 
CFLAGS  = -W -Wall -O2 -pipe -g
LDFLAGS = 
EXEC    = physlib.o
CLEANE  = 

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(OBJ)

%.o: %.c %.h
	$(CC) $(DEFINES) -o $@ -c $< $(CFLAGS)

clean:
	rm -fv *.o
