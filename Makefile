CC=gcc

LIBS = -lm
CCFLAGS= -Wall -ggdb -I./inc 
LDFLAGS=

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

EXEC=compilateur

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(EXEC).c $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

clean:
	rm -f src/*.o
	rm -f $(EXEC)

buildAndRun:
	make clean
	make
	cd test && sh -c "./testAll.sh"
