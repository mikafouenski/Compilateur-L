CC = gcc

SRCDIR = src
INCDIR = inc
OBJDIR = obj

LIBS = -lm 
CCFLAGS = -Wall -ggdb -I${INCDIR}

SRC = $(wildcard *.c)
OBJ2 = $(patsubst %.c, %.o, $(SRC))

OBJ = analyseur_lexical.o util.o analyseur_syntaxique.o premiers.o suivants.o \
syntabs.o affiche_arbre_abstrait.o analyseur_semantique.o dico.o

all: compilateur

compilateur: compilateur.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilateur compilateur.c $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilateur
