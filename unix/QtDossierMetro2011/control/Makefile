CC      = x86_64-pc-linux-gnu-gcc
DEFINES = -DCOLOR
CFLAGS  = -W -Wall -O2 -pipe -g $(DEFINES)
LDFLAGS = -O2 -Wl
EXEC    = Control

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
	rm -fv $(EXEC)
