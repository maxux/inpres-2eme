CC      = x86_64-pc-linux-gnu-gcc
DEFINES = -D COLOR -D DEBUG
CFLAGS  = -W -Wall -O2 -pipe -g
LDFLAGS = -lpthread
EXEC    = control
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
