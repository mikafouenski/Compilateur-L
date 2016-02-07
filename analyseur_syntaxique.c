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
        }
    } else if (est_suivant(uniteCourante, _listeDecFonctions_)) 
        return;
    printf("ERREUR\n");
    exit(EXIT_FAILURE);
}
void declarationVariable (void) {

}
void declarationFonction (void) {

}
void listeParam (void) {

}
void listeInstructions (void) {

}
void instruction (void) {

}
void instructionAffect (void) {

}
void instructionBloc (void) {

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
