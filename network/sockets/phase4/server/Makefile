CC      = gcc
DEFINES = 
CFLAGS  = -W -Wall -O2 -pipe -g
LDFLAGS = 
LINK    = ../common/physlib/physlib.o ../common/tcplib/tcplib.o
EXEC    = server
CLEANE  = 

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LINK) $(LDFLAGS) 

%.o: %.c %.h
	$(CC) $(DEFINES) -o $@ -c $< $(CFLAGS)

clean:
	rm -fv *.o

mrproper: clean
	rm -fv $(EXEC) $(CLEANE)
