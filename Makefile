CC = gcc

SRCDIR = src
INCDIR = inc
OBJDIR = obj

LIBS = -lm 
CCFLAGS = -Wall -ggdb -I${INCDIR}

SRC = $(wildcard *.c)
OBJ2 = $(patsubst %.c, %.o, $(SRC))

OBJ = analyseur_lexical.o util.o analyseur_syntaxique.o premiers.o suivants.o \
syntabs.o affiche_arbre_abstrait.o analyseur_semantique.o

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

syntabs.o: syntabs.c
	$(CC) $(CCFLAGS) -c $^

affiche_arbre_abstrait.o: affiche_arbre_abstrait.c
	$(CC) $(CCFLAGS) -c $^

analyseur_semantique.o: analyseur_semantique.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilateur

JETESTE:
	@echo 'SRC' $(SRC)
	@echo 'OBJ' $(OBJ2)
