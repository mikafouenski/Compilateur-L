#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "analyseur_semantique.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
    int sflag = 0;
    int lflag = 0;
    int aflag = 0;
    int tflag = 0;
    int c;
    n_prog *p = NULL;
    opterr = 0;

    while ((c = getopt (argc, argv, "slat")) != -1) {
        switch (c) {
            case 's':
                sflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 't':
                tflag = 1;
                break;
            case 'a':
                aflag = 1;
                break;
            case '?':
                fprintf (stderr, "Caractère d'option inconnu `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
        }
    }

    yyin = fopen(argv[optind], "r"); 
    if(yyin == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[optind]);
        return 1;
    }
    
    if (lflag) {
        test_yylex_internal(yyin);
        rewind(yyin);
    }
    p = syntaxe(sflag, aflag);
    semantique(p, tflag);
    return 0;
}
