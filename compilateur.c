#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
    int sflag = 0;
    int lflag = 0;
    int c;
    opterr = 0;

    while ((c = getopt (argc, argv, "slh")) != -1) {
        switch (c) {
            case 's':
                sflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 'h':
                printf("Ici reposent les usages... Solennellement\n");
                return 0;
            case '?':
                if (isprint (optopt))
                    fprintf (stderr, "option inconnue `-%c'.\n", optopt);
                else
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
        return 0;
    } else if (sflag) {
        syntaxe(1);
    }
    syntaxe(0);
    return 0;
}
