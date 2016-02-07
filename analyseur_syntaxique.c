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
    if (est_premier(uniteCourante, _optDecVariables_))
    {
        affiche_balise_ouvrante("programme", trace_xml);
        optDecVariables();
        if (est_premier(uniteCourante, _listeDecFonctions_))
        {
            listeDecFonctions();
            affiche_balise_fermante("programme", trace_xml);
            return;
        }
    }
}
void conjonctionBis (void) {
    if (uniteCourante == ET)
    {
        affiche_balise_ouvrante("conjonction", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _negation_))
        {
            negation();
            if (est_premier(uniteCourante, _conjonctionBis_))
            {
                conjonctionBis();
                affiche_balise_fermante("conjonctionBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _conjonctionBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void optTailleTableau (void) {
    if (uniteCourante == CROCHET_OUVRANT)
    {
        affiche_balise_ouvrante("optTailleTableau", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (uniteCourante == NOMBRE)
        {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (uniteCourante == CROCHET_FERMANT)
            {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("optTailleTableau", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _optTailleTableau_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void expArithBis (void) {
    if (uniteCourante == PLUS)
    {
        affiche_balise_ouvrante("expArithBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _terme_))
        {
            terme();
            if (est_premier(uniteCourante, _expArithBis_))
            {
                expArithBis();
                affiche_balise_fermante("expArithBis", trace_xml);
                return;
            }
        }
    }
    else if (uniteCourante == MOINS)
    {
        affiche_balise_ouvrante("expArithBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _terme_))
        {
            terme();
            if (est_premier(uniteCourante, _expArithBis_))
            {
                expArithBis();
                affiche_balise_fermante("expArithBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _expArithBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void optSinon (void) {
    if (uniteCourante == SINON)
    {
        affiche_balise_ouvrante("optSinon", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _instructionBloc_))
        {
            instructionBloc();
            affiche_balise_fermante("optSinon", trace_xml);
            return;
        }
    }
    else if (est_suivant(uniteCourante, _optSinon_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void comparaisonBis (void) {
    if (uniteCourante == EGAL)
    {
        affiche_balise_ouvrante("comparaisonBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_))
        {
            expression();
            if (est_premier(uniteCourante, _comparaisonBis_))
            {
                comparaisonBis();
                affiche_balise_fermante("conjonctionBis", trace_xml);
                return;
            }
        }
    }
    else if (uniteCourante == INFERIEUR)
    {
        affiche_balise_ouvrante("comparaisonBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_))
        {
            expression();
            if (est_premier(uniteCourante, _comparaisonBis_))
            {
                comparaisonBis();
                affiche_balise_fermante("conjonctionBis", trace_xml);
                return;
            }
        }   
    }
    else if (est_suivant(uniteCourante, _comparaisonBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void optDecVariables (void) {
    if (est_premier(uniteCourante, _listeDecVariables_)
    {
        affiche_balise_ouvrante("optDecVariables", trace_xml);
        listeDecVariables();
        if (uniteCourante == POINT_VIRGULE)
        {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            affiche_balise_fermante("optDecVariables", trace_xml);
            return;
        }
    }
    else if (est_suivant(uniteCourante, _optDecVariables_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void optIndice (void) {
    if (uniteCourante == CROCHET_OUVRANT)
    {
        affiche_balise_ouvrante("optIndice", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_))
        {
            expression();
            if (uniteCourante == CROCHET_FERMANT)
            {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("optIndice", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _optIndice_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void listeDecVariablesBis (void) {
    if (uniteCourante == VIRGULE)
    {
        affiche_balise_ouvrante("listeDecVariablesBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _declarationVariable_))
        {
            declarationVariable();
            if (est_premier(uniteCourante, _listeDecVariablesBis_))
            {
                listeDecVariablesBis();
                affiche_balise_fermante("listeDecVariablesBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _listeDecVariablesBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void termeBis (void) {
    if (uniteCourante == FOIS)
    {
        affiche_balise_ouvrante("termeBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _facteur_))
        {
            facteur();
            if (est_premier(uniteCourante, _termeBis_))
            {
                termeBis();
                affiche_balise_fermante("termeBis", trace_xml);
                return;
            }
        }
    }
    else if (uniteCourante == DIVISE)
    {
        affiche_balise_ouvrante("termeBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _facteur_))
        {
            facteur();
            if (est_premier(uniteCourante, _termeBis_))
            {
                termeBis();
                affiche_balise_fermante("termeBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _termeBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void expressionBis (void) {
    if (uniteCourante == OU)
    {
        affiche_balise_ouvrante("expressionBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _conjonction_))
        {
            conjonction();
            if (est_premier(uniteCourante, _expressionBis_))
            {
                expressionBis();
                affiche_balise_fermante("expressionBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _expressionBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void instructionFaire (void) {

}
void optListeDecVariables (void) {
    if (est_premier(uniteCourante, _listeDecVariables_))
    {
        affiche_balise_ouvrante("optListeDecVariables", trace_xml);
        listeDecVariables();
        affiche_balise_fermante("optListeDecVariables", trace_xml);
        return
    }
    else if (est_suivant(uniteCourante, _listeDecVariables_))
        return;
    printf("ERREUR\n");
    exit(-1);
}

void syntaxe(void) {
    initialise_premiers();
    initialise_suivants();
    uniteCourante = yylex();
    expression();
}