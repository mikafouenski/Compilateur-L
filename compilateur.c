#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int lflg, sflg;
int opterr = 0;

int main(int argc, char **argv) {

    int c;
    if (argc - 1 != 2) {
        printf("Met -l ou -s avant le fichier !\n");
        exit(1);
    }
    while ((c=getopt(argc, argv, "sl")) != EOF) switch(c) {
        case 's':
            sflg++;
            continue;

        case 'l':
            lflg++;
            continue;

        case '?':
            opterr++;
            continue;
        default:
            opterr++;
            break;
    }
    if(opterr) {
        fprintf(stderr,"usage: compilateur -sl file ...\n");
        exit(1);
    }
    yyin = fopen(argv[2], "r");
    if(yyin == NULL){
        fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[2]);
        exit(1);
    }
    if (lflg)
        test_yylex_internal(yyin);
    if (sflg)
        syntaxe();
  return 0;
}
