#include <stdlib.h>
#include "syntabs.h"
#include "dico.h"

void free_dico();
void free_n_prog (n_prog *n);
void free_l_instr(n_l_instr *n);
void free_instr(n_instr *n);
void free_instr_si(n_instr *n);
void free_instr_tantque(n_instr *n);
void free_instr_faire(n_instr *n);
void free_instr_pour(n_instr *n);
void free_instr_affect(n_instr *n);
void free_instr_appel(n_instr *n);
void free_instr_retour(n_instr *n);
void free_instr_ecrire(n_instr *n);
void free_l_exp(n_l_exp *n);
void free_exp(n_exp *n);
void free_varExp(n_exp *n);
void free_opExp(n_exp *n);
void free_intExp(n_exp *n);
void free_lireExp(n_exp *n);
void free_appelExp(n_exp *n);
void free_l_dec(n_l_dec *n);
void free_dec(n_dec *n);
void free_foncDec(n_dec *n);
void free_varDec(n_dec *n);
void free_tabDec(n_dec *n);
void free_var(n_var *n);
void free_var_simple(n_var *n);
void free_var_indicee(n_var *n);
void free_appel(n_appel *n);

void freedom (n_prog *n) {
  free_n_prog(n);
  free_dico();
}

void free_dico() {
  for (int i = 0; i < dico.sommet; ++i) {
    free (dico.tab[i].identif);
  }
}

void free_n_prog (n_prog *n) {
  free_l_dec(n->variables);
  free_l_dec(n->fonctions);
  free(n);
}


void free_l_instr(n_l_instr *n) {
  if(n){
  free_instr(n->tete);
  free_l_instr(n->queue);
  free(n);
  }
}


void free_instr(n_instr *n) {
  if(n){
    if(n->type == blocInst) {
      free_l_instr(n->u.liste);
      free(n);
    }
    else if(n->type == affecteInst) free_instr_affect(n);
    else if(n->type == siInst) free_instr_si(n);
    else if(n->type == tantqueInst) free_instr_tantque(n);
    else if(n->type == faireInst) free_instr_faire(n);
    else if(n->type == pourInst) free_instr_pour(n);
    else if(n->type == appelInst) free_instr_appel(n);
    else if(n->type == retourInst) free_instr_retour(n);
    else if(n->type == ecrireInst) free_instr_ecrire(n);
  }
}


void free_instr_si(n_instr *n) {
  free_exp(n->u.si_.test);
  free_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    free_instr(n->u.si_.sinon);
  }
  free(n);
}


void free_instr_tantque(n_instr *n) {
  free_exp(n->u.tantque_.test);
  free_instr(n->u.tantque_.faire);
  free(n);
}


void free_instr_faire(n_instr *n) {
  free_instr(n->u.faire_.faire);
  free_exp(n->u.faire_.test);
  free(n);
}


void free_instr_pour(n_instr *n) {
  free_instr(n->u.pour_.init);
  free_exp(n->u.pour_.test);
  free_instr(n->u.pour_.faire);
  free_instr(n->u.pour_.incr);
  free(n);
}


void free_instr_affect(n_instr *n) {
  free_var(n->u.affecte_.var);
  free_exp(n->u.affecte_.exp);
  free(n);
}


void free_instr_appel(n_instr *n) {
  free_appel(n->u.appel);
  free(n);
}

void free_appel(n_appel *n) {
  free_l_exp(n->args);
  free(n);
}


void free_instr_retour(n_instr *n) {
  free_exp(n->u.retour_.expression);
  free(n);
}


void free_instr_ecrire(n_instr *n) {
  free_exp(n->u.ecrire_.expression);
  free(n);
}


void free_l_exp(n_l_exp *n) {
  if(n){
    free_exp(n->tete);
    free_l_exp(n->queue);
  }
  free(n);
}

void free_exp(n_exp *n) {
  if(n->type == varExp) free_varExp(n);
  else if(n->type == opExp) free_opExp(n);
  else if(n->type == intExp) free_intExp(n);
  else if(n->type == appelExp) free_appelExp(n);
  else if(n->type == lireExp) free_lireExp(n);
}

void free_varExp(n_exp *n) {
  free_var(n->u.var);
  free(n);
}

void free_opExp(n_exp *n) {
  if( n->u.opExp_.op1 != NULL ) {
    free_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    free_exp(n->u.opExp_.op2);
  }
  free(n);
}


void free_intExp(n_exp *n) {
  free(n);
}

void free_lireExp(n_exp *n) {
  free(n);
}


void free_appelExp(n_exp *n) {
  free_appel(n->u.appel);
  free(n);
}


void free_l_dec(n_l_dec *n) {
  if( n ){
    free_dec(n->tete);
    free_l_dec(n->queue);
  }
  free(n);
}


void free_dec(n_dec *n) {
  if(n){
    if(n->type == foncDec) {
      free_foncDec(n);
    }
    else if(n->type == varDec) {
      free_varDec(n);
    }
    else if(n->type == tabDec) { 
      free_tabDec(n);
    }
  }
}


void free_foncDec(n_dec *n) {
  free_l_dec(n->u.foncDec_.param);
  free_l_dec(n->u.foncDec_.variables);
  free_instr(n->u.foncDec_.corps);
  free(n->nom);
  free(n);
}


void free_varDec(n_dec *n) {
  free(n->nom);
free(n);
}


void free_tabDec(n_dec *n) {
  free(n->nom);
  free(n);
}


void free_var(n_var *n) {
  if(n->type == simple) {
    free_var_simple(n);
  }
  else if(n->type == indicee) {
    free_var_indicee(n);
  }
}

void free_var_simple(n_var *n) {
  free(n->nom);
  free(n);
}

void free_var_indicee(n_var *n) {
  free_exp( n->u.indicee_.indice );
  free(n->nom);
  free(n);
}
