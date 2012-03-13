UNAME := $(shell uname)

ifeq ($(UNAME), Linux)

CC      = gcc
CFLAGS  = -lrt -O2 -W -Wall -lpthread
LDFLAGS = -lrt

endif
ifeq ($(UNAME), QNX)

CC      = qcc
CFLAGS  = -O2 -W -Wall -lpthread
LDFLAGS = 

endif

DEFINES =
EXEC    = server
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
