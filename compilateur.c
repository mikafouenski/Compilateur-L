#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inc/affiche_arbre_abstrait.h"

#include "inc/analyseur_lexical.h"
#include "inc/analyseur_syntaxique.h"
#include "inc/analyseur_semantique.h"

#include "inc/symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
    int sflag = 0;
    int lflag = 0;
    int aflag = 0;
    int tflag = 0;
    int c;
    opterr = 0;

    n_prog *p = NULL;

    while ((c = getopt (argc, argv, "slta")) != -1) {
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
                fprintf (stderr, "Option inconnu `\\x%x'.\n", optopt);
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

    p = syntaxe(sflag);

    if (aflag) affiche_n_prog(p);

    semantique(p, tflag);
    return 0;
}
