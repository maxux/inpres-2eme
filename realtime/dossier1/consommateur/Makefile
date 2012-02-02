UNAME := $(shell uname)

ifeq ($(UNAME), Linux)

CC      = gcc
CFLAGS  = -lrt -W -Wall -O2
LDFLAGS = -lrt -W -Wall -O2

endif
ifeq ($(UNAME), QNX)

CC      = qcc
CFLAGS  = -W -Wall -O2
LDFLAGS = -W -Wall -O2

endif

DEFINES =
EXEC    = consommateur
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
