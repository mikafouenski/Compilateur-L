#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

void analyse_n_prog(n_prog *n);
void analyse_l_instr(n_l_instr *n);
void analyse_instr(n_instr *n);
void analyse_instr_si(n_instr *n);
void analyse_instr_tantque(n_instr *n);
void analyse_instr_faire(n_instr *n);      /* MODIFIE POUR EVAL */
void analyse_instr_pour(n_instr *n);       /* MODIFIE POUR EVAL */
void analyse_instr_affect(n_instr *n);
void analyse_instr_appel(n_instr *n);
void analyse_instr_retour(n_instr *n);
void analyse_instr_ecrire(n_instr *n);
void analyse_l_exp(n_l_exp *n);
void analyse_exp(n_exp *n);
void analyse_varExp(n_exp *n);
void analyse_opExp(n_exp *n);
void analyse_intExp(n_exp *n);
void analyse_lireExp(n_exp *n);
void analyse_appelExp(n_exp *n);
void analyse_l_dec(n_l_dec *n);
void analyse_dec(n_dec *n);
void analyse_foncDec(n_dec *n);
void analyse_varDec(n_dec *n);
void analyse_tabDec(n_dec *n);
void analyse_var(n_var *n);
void analyse_var_simple(n_var *n);
void analyse_var_indicee(n_var *n);
void analyse_appel(n_appel *n);

void semantique(n_prog *p, int trace_dico) {
  analyse_n_prog(p);
  if (trace_dico) affiche_dico();
}


int contexte = C_VARIABLE_GLOBALE;
int adresseLocaleCourante = 0;
int adresseArgumentCourant = 0;

/*-------------------------------------------------------------------------*/

void analyse_n_prog(n_prog *n)
{
  analyse_l_dec(n->variables);
  analyse_l_dec(n->fonctions);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void analyse_l_instr(n_l_instr *n)
{
  if(n){
  analyse_instr(n->tete);
  analyse_l_instr(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) analyse_l_instr(n->u.liste);
    else if(n->type == affecteInst) analyse_instr_affect(n);
    else if(n->type == siInst) analyse_instr_si(n);
    else if(n->type == tantqueInst) analyse_instr_tantque(n);
    else if(n->type == faireInst) analyse_instr_faire(n);
    else if(n->type == pourInst) analyse_instr_pour(n);    
    else if(n->type == appelInst) analyse_instr_appel(n);
    else if(n->type == retourInst) analyse_instr_retour(n);
    else if(n->type == ecrireInst) analyse_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr_si(n_instr *n)
{  
  analyse_exp(n->u.si_.test);
  analyse_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    analyse_instr(n->u.si_.sinon);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr_tantque(n_instr *n)
{
  analyse_exp(n->u.tantque_.test);
  analyse_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  analyse_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  analyse_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void analyse_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
  analyse_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  analyse_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  analyse_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  analyse_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void analyse_instr_affect(n_instr *n)
{
  analyse_var(n->u.affecte_.var);
  analyse_exp(n->u.affecte_.exp);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_appel(n_instr *n)
{
  analyse_appel(n->u.appel);
}
/*-------------------------------------------------------------------------*/

void analyse_appel(n_appel *n)
{
  analyse_l_exp(n->args);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_retour(n_instr *n)
{
  analyse_exp(n->u.retour_.expression);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_ecrire(n_instr *n)
{
  analyse_exp(n->u.ecrire_.expression);
}

/*-------------------------------------------------------------------------*/

void analyse_l_exp(n_l_exp *n)
{
  if(n){
    analyse_exp(n->tete);
    analyse_l_exp(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_exp(n_exp *n)
{
  if(n->type == varExp) analyse_varExp(n);
  else if(n->type == opExp) analyse_opExp(n);
  else if(n->type == intExp) analyse_intExp(n);
  else if(n->type == appelExp) analyse_appelExp(n);
  else if(n->type == lireExp) analyse_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void analyse_varExp(n_exp *n)
{
  analyse_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
void analyse_opExp(n_exp *n)
{
  /*if(n->u.opExp_.op == plus) analyse_texte("plus", trace_abs);
  else if(n->u.opExp_.op == moins) analyse_texte("moins", trace_abs);
  else if(n->u.opExp_.op == fois) analyse_texte("fois", trace_abs);
  else if(n->u.opExp_.op == divise) analyse_texte("divise", trace_abs);
  else if(n->u.opExp_.op == egal) analyse_texte("egal", trace_abs);
  else if(n->u.opExp_.op == diff) analyse_texte("diff", trace_abs);
  else if(n->u.opExp_.op == inf) analyse_texte("inf", trace_abs);
  else if(n->u.opExp_.op == infeg) analyse_texte("infeg", trace_abs);
  else if(n->u.opExp_.op == ou) analyse_texte("ou", trace_abs);
  else if(n->u.opExp_.op == et) analyse_texte("et", trace_abs);
  else if(n->u.opExp_.op == non) analyse_texte("non", trace_abs);*/
  if( n->u.opExp_.op1 != NULL ) {
    analyse_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    analyse_exp(n->u.opExp_.op2);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_intExp(n_exp *n)
{
}

/*-------------------------------------------------------------------------*/
void analyse_lireExp(n_exp *n)
{
}

/*-------------------------------------------------------------------------*/

void analyse_appelExp(n_exp *n)
{
  analyse_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void analyse_l_dec(n_l_dec *n)
{
  if( n ){
    analyse_dec(n->tete);
    analyse_l_dec(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_dec(n_dec *n)
{

  if(n){
    if(n->type == foncDec) {
      analyse_foncDec(n);
    }
    else if(n->type == varDec) {
      analyse_varDec(n);
    }
    else if(n->type == tabDec) { 
      analyse_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

void analyse_foncDec(n_dec *n)
{
  if (rechercheExecutable(n->nom) == -1) {
    ajouteIdentificateur(n->nom, contexte, T_FONCTION, adresseLocaleCourante, 0);
    adresseLocaleCourante += 4;
    entreeFonction();
    analyse_l_dec(n->u.foncDec_.param);
    analyse_l_dec(n->u.foncDec_.variables);
    analyse_instr(n->u.foncDec_.corps);
    sortieFonction();
  }
}

/*-------------------------------------------------------------------------*/

void analyse_varDec(n_dec *n)
{
  if (rechercheDeclarative(n->nom) == -1) {
    ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseLocaleCourante, -1);
    adresseLocaleCourante += 4;
  }
}

/*-------------------------------------------------------------------------*/

void analyse_tabDec(n_dec *n)
{
  if (rechercheDeclarative(n->nom) == -1) {
    ajouteIdentificateur(n->nom, contexte, T_TABLEAU_ENTIER, adresseLocaleCourante, n->u.tabDec_.taille);
    adresseLocaleCourante += 4;
  }
}

/*-------------------------------------------------------------------------*/

void analyse_var(n_var *n)
{
  if(n->type == simple) {
    analyse_var_simple(n);
  }
  else if(n->type == indicee) {
    analyse_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void analyse_var_simple(n_var *n)
{
}

/*-------------------------------------------------------------------------*/
void analyse_var_indicee(n_var *n)
{
  analyse_exp( n->u.indicee_.indice );
}
/*-------------------------------------------------------------------------*/
