UNAME := $(shell uname)

ifeq ($(UNAME), Linux)

CC      = gcc
CFLAGS  = -lrt -W -Wall
LDFLAGS = -lrt

endif
ifeq ($(UNAME), QNX)

CC      = qcc
CFLAGS  = -W -Wall
LDFLAGS = 

endif

DEFINES =
EXEC    = consom
CLEANE  =

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(DEFINES) -o $@ -c $< $(CFLAGS)

clean:
	rm -fv *.o

mrproper: clean
	rm -fv $(EXEC) $(CLEANE)
