CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o util.o analyseur_syntaxique.o premiers.o suivants.o

all: compilateur

compilateur: compilateur.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilateur compilateur.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c $^

premiers.o: premiers.c
	$(CC) $(CCFLAGS) -c $^

suivants.o: suivants.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilateur
