#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

int contexte = C_VARIABLE_GLOBALE;
int adresseLocaleCourante = 0;
int adresseArgumentCourant = 0;
int trace_tab;
int trace_mips;
int nb_args_function;
int nb_var_local;

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

void semantique(n_prog *p, int trace_dico, int print_mips) {
  trace_tab = trace_dico;
  trace_mips = print_mips;
  analyse_n_prog(p);
}

int taille_n_l_dec(n_l_dec *liste) {
  if (liste) return 1 + taille_n_l_dec(liste->queue);
  return 0;
}

void mips_debut_fonction() {
  if (trace_mips) {
    printf("\tsubi $sp, $sp, 4    # debut fonction\n");
    printf("\tsw $fp, ($sp)\n");
    printf("\tmove $fp, $sp\n");
    printf("\tsubi $sp, $sp, 4\n");
    printf("\tsw $ra, ($sp)       # debut fonction\n");
  }
}

void mips_fin_function() {
  if (trace_mips) {
    printf("\tlw $ra, ($sp)       # fin fonction\n");
    printf("\taddi $sp\n");
    printf("\tlw $fp, ($sp)\n");
    printf("\taddi $sp, $sp, 4\n");
    printf("\tjr $ra              # fin fonction\n");
  }
}

void mips_empile(int v) {
  if (trace_mips) {
    printf("\tsubi $sp, $sp, 4    # Empile\n");
    printf("\tsw $t%d, ($sp)\n", v);
  }
}

void mips_depile(int v) {
  if (trace_mips) {
    printf("\tlw $t%d, ($sp)       # Depile\n", v);
    printf("\taddi $sp, $sp, 4\n");
  }
}

/*-------------------------------------------------------------------------*/

void analyse_n_prog(n_prog *n) {
  if (trace_mips) printf("\t.data\n");
  analyse_l_dec(n->variables);
  if (trace_mips) {
    printf("\t.text\n");
    printf("__start:\n\tjal main\n\tli $v0, 10\n\tsyscall\n");
  }
  analyse_l_dec(n->fonctions);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void analyse_l_instr(n_l_instr *n) {
  if(n){
  analyse_instr(n->tete);
  analyse_l_instr(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr(n_instr *n) {
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

void analyse_instr_si(n_instr *n) {
  analyse_exp(n->u.si_.test);
  analyse_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    analyse_instr(n->u.si_.sinon);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr_tantque(n_instr *n) {
  analyse_exp(n->u.tantque_.test);
  analyse_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_faire(n_instr *n) {
  analyse_instr(n->u.faire_.faire);
  analyse_exp(n->u.faire_.test);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_pour(n_instr *n) {
  analyse_instr(n->u.pour_.init);
  analyse_exp(n->u.pour_.test);
  analyse_instr(n->u.pour_.faire);
  analyse_instr(n->u.pour_.incr);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_affect(n_instr *n) {
  analyse_var(n->u.affecte_.var);
  analyse_exp(n->u.affecte_.exp);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_appel(n_instr *n) {
  analyse_appel(n->u.appel);
}
/*-------------------------------------------------------------------------*/

void analyse_appel(n_appel *n) {
  analyse_l_exp(n->args);
  if (trace_mips) {
    printf("\tjal %s\n", n->fonction);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_instr_retour(n_instr *n) {
  analyse_exp(n->u.retour_.expression);
}

/*-------------------------------------------------------------------------*/

void analyse_instr_ecrire(n_instr *n) {
  analyse_exp(n->u.ecrire_.expression);
  if (trace_mips) {
    printf("\tmove $a0, $t0\n\tli $v0 4\n\tsyscall\n");
  }
}

/*-------------------------------------------------------------------------*/

void analyse_l_exp(n_l_exp *n) {
  if(n){
    analyse_exp(n->tete);
    analyse_l_exp(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_exp(n_exp *n) {
  if(n->type == varExp) analyse_varExp(n);
  else if(n->type == opExp) analyse_opExp(n);
  else if(n->type == intExp) analyse_intExp(n);
  else if(n->type == appelExp) analyse_appelExp(n);
  else if(n->type == lireExp) analyse_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void analyse_varExp(n_exp *n) {
  analyse_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
void analyse_opExp(n_exp *n) {
  if( n->u.opExp_.op1 != NULL ) {
    analyse_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    analyse_exp(n->u.opExp_.op2);
  }
  if(n->u.opExp_.op == plus) {
    mips_depile(0);
    mips_depile(1);
    if (trace_mips) {
      printf("\tadd $t0, $t1, $t0\n");
      mips_empile(0);
    }
  }
  else if(n->u.opExp_.op == moins) ;
  else if(n->u.opExp_.op == fois) ;
  else if(n->u.opExp_.op == divise) ;
  else if(n->u.opExp_.op == egal) ;
  else if(n->u.opExp_.op == diff) ;
  else if(n->u.opExp_.op == inf) ;
  else if(n->u.opExp_.op == infeg) ;
  else if(n->u.opExp_.op == ou) ;
  else if(n->u.opExp_.op == et) ;
  else if(n->u.opExp_.op == non) ;
}

/*-------------------------------------------------------------------------*/

void analyse_intExp(n_exp *n) {
  if (trace_mips) {
    printf("\tli $t0, %d\n", n->u.entier);
    mips_empile(0);
  }
}

/*-------------------------------------------------------------------------*/
void analyse_lireExp(n_exp *n) {
}

/*-------------------------------------------------------------------------*/

void analyse_appelExp(n_exp *n) {
  analyse_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void analyse_l_dec(n_l_dec *n) {
  if( n ){
    analyse_dec(n->tete);
    analyse_l_dec(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void analyse_dec(n_dec *n) {

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

void analyse_foncDec(n_dec *n) {
  if (rechercheExecutable(n->nom) == -1) {
    if (trace_mips) printf("%s:\n", n->nom);
    ajouteIdentificateur(n->nom, contexte, T_FONCTION, 0, taille_n_l_dec(n->u.foncDec_.param));
    dico.base = dico.base + 1;
    entreeFonction();
    mips_debut_fonction();
    contexte = C_ARGUMENT;
    nb_args_function = taille_n_l_dec(n->u.foncDec_.param);
    analyse_l_dec(n->u.foncDec_.param);
    contexte = C_VARIABLE_LOCALE;
    // for (int i = 0; i < taille_n_l_dec(n->u.foncDec_.param); ++i) {
    //   if (trace_mips) {
    //     printf("\tlw $t%d, %d($fp)\n", creg, 4 * ());
    //   }
    // }
    nb_var_local = taille_n_l_dec(n->u.foncDec_.variables);
    if (trace_mips && nb_var_local > 0) {
      printf("\tsubi $sp, $sp, %d\n", 4 * (nb_var_local + 1));
    }
    analyse_l_dec(n->u.foncDec_.variables);
    analyse_instr(n->u.foncDec_.corps);
    if (trace_tab) affiche_dico();
    mips_depile(0);
    if (trace_mips) {
      printf("\tsw $t0, %d($fp)\n", 4 * (nb_args_function + 1));
    }
    mips_fin_function();
    sortieFonction();
  }
}

/*-------------------------------------------------------------------------*/

void analyse_varDec(n_dec *n) {
  if (rechercheDeclarative(n->nom) == -1) {
    if (contexte == C_VARIABLE_GLOBALE || contexte == C_VARIABLE_LOCALE) {
      ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseLocaleCourante, -1);
      if (contexte == C_VARIABLE_GLOBALE) {
        dico.base = dico.base + 1;
        if (trace_mips) printf("%s :\t.space\t4\n", n->nom);
      }
      adresseLocaleCourante += 4;
    } else if (contexte == C_ARGUMENT) {
      ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseArgumentCourant, -1);
      adresseArgumentCourant += 4;
    }
  }
}

/*-------------------------------------------------------------------------*/

void analyse_tabDec(n_dec *n) {
  if (rechercheDeclarative(n->nom) == -1) {
    ajouteIdentificateur(n->nom, contexte, T_TABLEAU_ENTIER, adresseLocaleCourante, n->u.tabDec_.taille);
    dico.base = dico.base + 1;
    adresseLocaleCourante += 4 * n->u.tabDec_.taille;
  }
}

/*-------------------------------------------------------------------------*/

void analyse_var(n_var *n) {
  if(n->type == simple) {
    analyse_var_simple(n);
  }
  else if(n->type == indicee) {
    analyse_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void analyse_var_simple(n_var *n) {
  if (trace_mips) {
    if (nb_args_function > 0) {
      printf("\tlw t0, %d($fp)\n", 4 * nb_args_function);
      mips_empile(0);
    }
  }
}

/*-------------------------------------------------------------------------*/
void analyse_var_indicee(n_var *n) {
  analyse_exp( n->u.indicee_.indice );
}
/*-------------------------------------------------------------------------*/
