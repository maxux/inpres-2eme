CC      = x86_64-pc-linux-gnu-g++
CFLAGS  = -O2 -pipe -g
LDFLAGS = 
EXEC    = main1

SRC=$(wildcard *.cxx)
OBJ=$(SRC:.cxx=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.cxx %.hxx
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -v *.o

mrproper: clean
	rm -v $(EXEC)
