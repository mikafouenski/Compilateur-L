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
            if (est_premier(uniteCourante, _optTailleTableau_)) {
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
    if (uniteCourante == SI) {
        affiche_balise_ouvrante("instructionSi", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_)) {
            expression();
            if (uniteCourante == ALORS) {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                if (est_premier(uniteCourante, _instructionBloc_)) {
                    instructionBloc();
                    if (est_premier(uniteCourante, _optSinon_)) {
                        optSinon();
                        affiche_balise_fermante("instructionSi", trace_xml);
                        return;
                    }  else {
                        printf("ERREUR\n");
                        exit(EXIT_FAILURE);
                    }
                }  else {
                    printf("ERREUR\n");
                    exit(EXIT_FAILURE);
                }
            }  else {
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
void instructionTantque (void) {
    if (uniteCourante == TANTQUE) {
        affiche_balise_ouvrante("instructionTantque", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_)) {
            expression();
            if (uniteCourante == FAIRE) {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                if (est_premier(uniteCourante, _instructionBloc_)) {
                    instructionBloc();
                    affiche_balise_fermante("instructionTantque", trace_xml);
                    return;
                }  else {
                    printf("ERREUR\n");
                    exit(EXIT_FAILURE);
                }
            }  else {
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
void instructionAppel (void) {
    if (est_premier(uniteCourante, _appelFct_)) {
        affiche_balise_ouvrante("instructionAppel", trace_xml);
        appelFct();
        if (uniteCourante == POINT_VIRGULE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            affiche_balise_fermante("instructionAppel", trace_xml);
            return;
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instructionRetour (void) {
    if (uniteCourante == RETOUR) {
        affiche_balise_ouvrante("instructionRetour", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_)) {
            expression();
            if (uniteCourante == POINT_VIRGULE) {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("instructionRetour", trace_xml);
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
void instructionEcriture (void) {
    if (uniteCourante == ECRIRE) {
        affiche_balise_ouvrante("instructionEcriture", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (uniteCourante == PARENTHESE_OUVRANTE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (est_premier(uniteCourante, _expression_)) {
                expression();
                if (uniteCourante == PARENTHESE_FERMANTE) {
                    nom_token(uniteCourante, nom, valeur);
                    affiche_element(nom, valeur, trace_xml);
                    uniteCourante = yylex();
                    if (uniteCourante == POINT_VIRGULE) {
                        nom_token(uniteCourante, nom, valeur);
                        affiche_element(nom, valeur, trace_xml);
                        uniteCourante = yylex();
                        affiche_balise_fermante("instructionEcriture", trace_xml);
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
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void instructionVide (void) {
    if (uniteCourante == POINT_VIRGULE) {
        affiche_balise_ouvrante("instructionVide", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        affiche_balise_fermante("instructionVide", trace_xml);
        return;
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void var (void) {
    if (uniteCourante == ID_VAR) {
        affiche_balise_ouvrante("var", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _optIndice_)) {
            optIndice();
            affiche_balise_fermante("var", trace_xml);
            return;
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void expression (void) {
    if (est_premier(uniteCourante, _conjonction_)) {
        affiche_balise_ouvrante("expression", trace_xml);
        conjonction();
        if (est_premier(uniteCourante, _expressionBis_)) {
            expressionBis();
            affiche_balise_fermante("expression", trace_xml);
            return;
        } else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void appelFct (void) {
    if (uniteCourante == ID_FCT) {
        affiche_balise_ouvrante("appelFct", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (uniteCourante == PARENTHESE_OUVRANTE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (est_premier(uniteCourante, _listeExpressions_)) {
                listeExpressions();
                if (uniteCourante == PARENTHESE_FERMANTE) {
                    nom_token(uniteCourante, nom, valeur);
                    affiche_element(nom, valeur, trace_xml);
                    uniteCourante = yylex();
                    affiche_balise_fermante("appelFct", trace_xml);
                    return;
                }  else {
                    printf("ERREUR\n");
                    exit(EXIT_FAILURE);
                }
            }  else {
                printf("ERREUR\n");
                exit(EXIT_FAILURE);
            }
        }  else {
            printf("ERREUR\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void conjonction (void) {
    if (est_premier(uniteCourante, _negation_))
    {
        affiche_balise_ouvrante("conjonction", trace_xml);
        negation();
        if (est_premier(uniteCourante, _conjonctionBis_))
        {
            conjonctionBis();
            affiche_balise_fermante("conjonction", trace_xml);
            return;
        }
    }
}
void negation (void) {
    if (uniteCourante == NON)
    {
        affiche_balise_ouvrante("negation", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _comparaison_))
        {
            comparaison();
            affiche_balise_fermante("negation", trace_xml);
            return;
        }
    }
    else if (est_premier(uniteCourante, _comparaison_))
    {
        affiche_balise_ouvrante("negation", trace_xml);
        comparaison();
        affiche_balise_fermante("negation", trace_xml);
        return;
    }
}
void comparaison (void) {
    if (est_premier(uniteCourante, expArith))
    {
        affiche_balise_ouvrante("comparaison", trace_xml);
        expArith();
        if (est_premier(uniteCourante, _comparaisonBis_))
        {
            comparaisonBis();
            affiche_balise_fermante("comparaison", trace_xml);
            return;
        }
    }
}
void expArith (void) {
    if (est_premier(uniteCourante, _facteur_))
    {
        affiche_balise_ouvrante("expArith", trace_xml);
        facteur();
        if (est_premier(uniteCourante, _expArithBis_))
        {
            expArithBis();
            affiche_balise_fermante("expArith", trace_xml);
            return;
        }
    }
}
void terme (void) {
    if (est_premier(uniteCourante, _facteur_))
    {
        affiche_balise_ouvrante("terme", trace_xml);
        facteur();
        if (est_premier(uniteCourante, _termeBis_))
        {
            termeBis();
            affiche_balise_fermante("terme", trace_xml);
            return;
        }
    }
}
void facteur (void) {
    if (uniteCourante == PARENTHESE_OUVRANTE)
    {
        affiche_balise_ouvrante("facteur", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression__))
        {
            expression();
            if (uniteCourante == PARENTHESE_FERMANTE)
            {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("facteur", trace_xml);
                return;
            }
        }
    }
    else if (uniteCourante == NOMBRE)
    {
        affiche_balise_ouvrante("facteur", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        affiche_balise_fermante("facteur", trace_xml);
        return;
    }
    else if (est_premier(uniteCourante, _appelFct_))
    {
        affiche_balise_ouvrante("facteur", trace_xml);
        appelFct();
        affiche_balise_fermante("facteur", trace_xml);
        return;
    }
    else if (est_premier(uniteCourante, _var_))
    {
        affiche_balise_ouvrante("facteur", trace_xml);
        var();
        affiche_balise_fermante("facteur", trace_xml);
        return;
    }
    else if (uniteCourante == LIRE)
    {
        affiche_balise_ouvrante("facteur", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (uniteCourante == PARENTHESE_OUVRANTE)
        {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, trace_xml);
            uniteCourante = yylex();
            if (uniteCourante == PARENTHESE_FERMANTE)
            {
                nom_token(uniteCourante, nom, valeur);
                affiche_element(nom, valeur, trace_xml);
                uniteCourante = yylex();
                affiche_balise_fermante("facteur", trace_xml);
                return;
            }
        }
    }
}
void argumentsEffectifs (void) {

}
void listeExpressions (void) {
    if (est_premier(uniteCourante, _expression__))
    {
        affiche_balise_ouvrante("listeExpressions", trace_xml);
        expression();
        if (est_premier(uniteCourante, _listeExpressionsBis_))
        {
            listeExpressionsBis();
            affiche_balise_fermante("listeExpressions", trace_xml);
            return;
        }
    }
    else if (est_suivant(uniteCourante, _listeExpressions_))
        return;
    printf("ERREUR\n");
    exit(-1);
}
void listeExpressionsBis (void) {
    if (uniteCourante == VIRGULE)
    {
        affiche_balise_ouvrante("listeExpressionsBis", trace_xml);
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, trace_xml);
        uniteCourante = yylex();
        if (est_premier(uniteCourante, _expression_))
        {
            expression();
            if (est_premier(uniteCourante, _listeExpressionsBis_))
            {
                listeExpressionsBis();
                affiche_balise_fermante("listeExpressionsBis", trace_xml);
                return;
            }
        }
    }
    else if (est_suivant(uniteCourante, _listeExpressionsBis_))
        return;
    printf("ERREUR\n");
    exit(-1);
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
        } else {
            erreur("ERREUR");
            exit(EXIT_FAILURE);
        }
    } else {
        erreur("ERREUR");
        exit(EXIT_FAILURE);
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
    if (est_premier(uniteCourante, _listeDecVariables_))
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
        return;
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
