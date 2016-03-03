#ifndef __SYNTAX__
#define __SYNTAX__

#include "syntabs.h"

n_l_dec *listeDecVariables (void);
n_l_dec *listeDecFonctions (void);
n_dec *declarationVariable (void);
n_dec *declarationFonction (void);
n_l_dec *listeParam (void);
n_l_instr *listeInstructions (void);
n_instr *instruction (void);
n_instr *instructionAffect (void);
n_instr *instructionBloc (void);
n_instr *instructionSi (void);
n_instr *instructionTantque (void);
n_instr *instructionAppel (void);
n_instr *instructionRetour (void);
n_instr *instructionEcriture (void);
n_instr *instructionVide (void);
n_var *var (void);
n_exp *expression (void);
n_appel *appelFct (void);
n_exp *conjonction (void);
n_exp *negation (void);
n_exp *comparaison (void);
n_exp *expArith (void);
n_exp *terme (void);
n_exp *facteur (void);
n_exp *argumentsEffectifs (void);
n_l_exp *listeExpressions (void);
n_l_exp *listeExpressionsBis (n_l_exp *herite);
n_prog *programme (void);
n_exp *conjonctionBis (n_exp *herite);
int optTailleTableau (void);
n_exp *expArithBis (n_exp *herite);
n_instr *optSinon (void);
n_exp *comparaisonBis (n_exp *herite);
n_l_dec *optDecVariables (void);
n_exp *optIndice (void);
n_l_dec *listeDecVariablesBis (n_l_dec *herite);
n_exp *termeBis (n_exp *herite);
n_exp *expressionBis (n_exp *herite);
n_instr *instructionFaire (void);
n_l_dec *optListeDecVariables (void);
n_instr *instructionPour (void);

void EatTerminal(void);
void DisplayErreur(void);

void syntaxe(int trace_xml_tree, int trace_abs_tree);

#endif
