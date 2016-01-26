#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "analyseur_lexical.h"

int uniteCourante;

void expression(void) {
    affiche_balise_ouvrante("Expresion", 1);
    terme();
    expressionBis();
    affiche_balise_fermante("Expresion", 1);
}

void expressionBis(void) {
    affiche_balise_ouvrante("ExpresionBIS", 1);
    if(uniteCourante == PLUS) {
        uniteCourante = yylex();
        expression();
        affiche_balise_fermante("ExpresionBIS", 1);
    }
}

void terme(void) {
    affiche_balise_ouvrante("Terme", 1);
    facteur();
    termeBis();
    affiche_balise_fermante("Terme", 1);
}

void termeBis(void) {
    affiche_balise_ouvrante("TermeBIS", 1);
    if(uniteCourante == FOIS) {
        uniteCourante = yylex();
        terme();
        affiche_balise_fermante("TermeBIS", 1);
    }
}

void facteur(void) {
    affiche_balise_ouvrante("Facteur", 1);
    if(uniteCourante == PARENTHESE_OUVRANTE) {
        uniteCourante = yylex();
        expression();
        if (uniteCourante == PARENTHESE_FERMANTE) {
            uniteCourante = yylex();
            affiche_balise_fermante("Facteur", 1);
        }
        else {
            printf("Erreur de syntaxe 1");
            exit (-1);
        }
    }
    else {
        if (uniteCourante == NOMBRE) {
            uniteCourante = yylex();
            affiche_balise_fermante("Facteur", 1);
        } else {
            printf("Erreur de syntaxe 2");
            exit (-1);
        }
    }
}

void syntaxe(FILE *yyin) {
    uniteCourante = yylex();
    //nom_token(uniteCourante, nom, valeur);
    expression();
}
