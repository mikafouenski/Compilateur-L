#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "symboles.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "analyseur_lexical.h"
#include "premiers.h"
#include "suivants.h"

int uniteCourante;
int trace_xml = 1;
char nom[100];
char valeur[100];

/*void expression(void) {
    affiche_balise_ouvrante("Expression", trace_xml);
    terme();
    expressionBis();
    affiche_balise_fermante("Expression", trace_xml);
}

void expressionBis(void) {
    if(uniteCourante == PLUS) {
        affiche_balise_ouvrante("ExpresionBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        expression();
        affiche_balise_fermante("ExpresionBis", trace_xml);
    }
}

void terme(void) {
    affiche_balise_ouvrante("Terme", trace_xml);
    facteur();
    termeBis();
    affiche_balise_fermante("Terme", trace_xml);
}

void termeBis(void) {
    if(uniteCourante == FOIS) {
        affiche_balise_ouvrante("TermeBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        terme();
        affiche_balise_fermante("TermeBis", trace_xml);
    }
}

void facteur(void) {
    if(uniteCourante == PARENTHESE_OUVRANTE) {
        affiche_balise_ouvrante("Facteur", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        expression();
        if (uniteCourante == PARENTHESE_FERMANTE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            affiche_balise_fermante("Facteur", trace_xml);
        }
        else {
            erreur("Erreur de syntaxe 1.");
            exit (-1);
        }
    }
    else {
        if (uniteCourante == NOMBRE) {
            affiche_balise_ouvrante("Facteur", trace_xml);
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            affiche_balise_fermante("Facteur", trace_xml);
        } else {
            erreur("Erreur de syntaxe 2.");
            exit (-1);
        }
    }
}*/


void listeDecVariables (void) {
    if (est_premier(uniteCourante, _declarationVariable_)) {
        affiche_balise_ouvrante("listeDecVariables", trace_xml);
        //nom_token(uniteCourante, nom, valeur);
        //affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        declarationVariable();
        if (est_premier(uniteCourante, _listeDecVariablesBis_)) {
            listeDecVariablesBis();
            affiche_balise_fermante("listeDecVariables", trace_xml);
            return;
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void listeDecFonctions (void) {
    if (est_premier(uniteCourante, _declarationFonction_)) {
        affiche_balise_ouvrante("listeDecFonctions", trace_xml);
        uniteCourante = yylex();
        declarationFonction();
        if (est_premier(uniteCourante, _listeDecFonctions_)) {
            listeDecFonctions();
            affiche_balise_fermante("listeDecFonctions", trace_xml);
            return;
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    } else if (est_suivant(uniteCourante, _listeDecFonctions_)) 
        return;
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void declarationVariable (void) {
    if (uniteCourante == ENTIER) {
        affiche_balise_ouvrante("declarationVariable", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (uniteCourante == ID_VAR) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (est_premier(uniteCourante, optTailleTableau)) {
                optTailleTableau();
                affiche_balise_fermante("declarationVariable", trace_xml);
                return;
            } else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void declarationFonction (void) {
    if (uniteCourante == ID_FCT) {
        affiche_balise_ouvrante("declarationFonction", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _listeParam_)) {
            listeParam();
            if (est_premier(uniteCourante, _optDecVariables_)) {
                optDecVariables();
                if (est_premier(uniteCourante, _instructionBloc_)) {
                    instructionBloc();
                    affiche_balise_fermante("declarationFonction", trace_xml);
                    return;
                } else {
                    printf("ERREUR\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void listeParam (void) {
    if (uniteCourante == PARENTHESE_OUVRANTE) {
        affiche_balise_ouvrante("listeParam", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _optListeDecVariables_)) {
            optListeDecVariables();
            if (uniteCourante == PARENTHESE_FERMANTE) {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("listeParam", trace_xml);
                return;
            } else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void listeInstructions (void) {
    if (est_premier(uniteCourante, _instruction_)) {
        affiche_balise_ouvrante("listeInstructions", trace_xml);
        instruction();
        if (est_premier(uniteCourante, _listeInstructions_)) {
            listeInstructions();
            affiche_balise_fermante("listeInstructions", trace_xml);
            return;
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    } else if (est_suivant(uniteCourante, _listeInstructions_))
        return;
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instruction (void) {
    if (est_premier(uniteCourante, _instructionAffect_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionAffect();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionBloc_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionBloc();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionSi_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionSi();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionTantque_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionTantque();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionAppel_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionAppel();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionRetour_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionRetour();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionEcriture_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionEcriture();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    } else if (est_premier(uniteCourante, _instructionVide_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        instructionVide();
        affiche_balise_fermante("instruction", trace_xml);
        return;
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instructionAffect (void) {
    if (est_premier(uniteCourante, _var_)) {
        affiche_balise_ouvrante("instructionAffect", trace_xml);
        var();
        if (uniteCourante == EGAL) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (est_premier(uniteCourante, _expression_)) {
                expression();
                if (uniteCourante == POINT_VIRGULE) {
                    nom_token(uniteCourante, nom, valeur);
                    affiche_element(nom, valeur, trace_xml);
                    uniteCourante = yylex();
                    affiche_balise_fermante("instructionAffect", trace_xml);
                    return;
                } else {
                    printf("ERREUR\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instructionBloc (void) {
    if (uniteCourante == ACCOLADE_OUVRANTE) {
        affiche_balise_ouvrante("instructionBloc", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _listeInstructions_)) {
            listeInstructions();
            if (uniteCourante == ACCOLADE_FERMANTE) {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("instructionBloc", trace_xml);
                return;
            } else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instructionSi (void) {

}
void instructionTantque (void) {

}
void instructionAppel (void) {

}
void instructionRetour (void) {

}
void instructionEcriture (void) {

}
void instructionVide (void) {

}
void var (void) {

}
void expression (void) {

}
void appelFct (void) {

}
void conjonction (void) {

}
void negation (void) {

}
void comparaison (void) {

}
void expArith (void) {

}
void terme (void) {

}
void facteur (void) {

}
void argumentsEffectifs (void) {

}
void listeExpressions (void) {

}
void listeExpressionsBis (void) {

}
void programme (void) {

}
void conjonctionBis (void) {

}
void optTailleTableau (void) {

}
void expArithBis (void) {

}
void optSinon (void) {

}
void comparaisonBis (void) {

}
void optDecVariables (void) {

}
void optIndice (void) {

}
void listeDecVariablesBis (void) {

}
void termeBis (void) {

}
void expressionBis (void) {

}
void instructionFaire (void) {

}
void optListeDecVariables (void) {

}

void syntaxe(void) {
    initialise_premiers();
    initialise_suivants();
    uniteCourante = yylex();
    expression();
}
