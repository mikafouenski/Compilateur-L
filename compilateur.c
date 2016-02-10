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

  if (argc == 1) return 1;
  while ((c = getopt (argc, argv, "sl")) != -1) {
    switch (c) {
      case 's':
        sflag = 1;
        break;
      case 'l':
        lflag = 1;
        break;
      case 'h':
        printf("USAGE\n");
      case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort ();
    }
  }

  //printf ("sflag = %d, lflag = %d, optind = %d\n", sflag, lflag, optind);

  yyin = fopen(argv[optind], "r"); 
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[optind]);
    return 1;
  }

  if (lflag) {
    test_yylex_internal(yyin);
  }
  if (sflag) {
    syntaxe();
  }

  return 0;
}
