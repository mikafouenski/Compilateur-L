CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o util.o

all: compilateur

compilateur: compilateur.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilateur compilateur.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex