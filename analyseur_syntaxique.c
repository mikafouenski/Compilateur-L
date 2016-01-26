#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "analyseur_lexical.h"

int uniteCourante;

void _expression_(void) {
	_terme_();
	_expressionBis_();
}

void _expressionBis_(void) {
	if(uniteCourante == PLUS) {
		uniteCourante = yylex();
		_expression_();
	}
	else return;
}

void _terme_(void) {
	_facteur_();
	_termeBis_();
}

void _termeBis_(void) {
	if(uniteCourante == FOIS) {
		uniteCourante = yylex();
		_terme_();
	}
	else return;	
}

void _facteur_(void) {
	if(uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		_expression_();
		if (uniteCourante == PARENTHESE_FERMANTE) {
			uniteCourante = yylex();
		}
		else {
			printf("Erreur de syntaxe");
			exit (-1);
		}
	}
	else return;
}