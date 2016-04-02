CC=gcc

LIBS = -lm
CCFLAGS= -Wall -I./inc -g3
LDFLAGS=

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

EXEC=compilateur

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -fv src/*.o
	rm -fv $(EXEC)

buildAndRun:
	make clean
	make
	cd test && sh -c "./testAll.sh"
