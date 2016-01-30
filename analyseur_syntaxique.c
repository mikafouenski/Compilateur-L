#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "analyseur_lexical.h"

int uniteCourante;
int trace_xml = 1;
char nom[100];
char valeur[100];

void expression(void) {
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
}

void syntaxe(void) {
    uniteCourante = yylex();
    expression();
}
