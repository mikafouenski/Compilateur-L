#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_semantique.h"
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
int etiquette = 0;
int asreturn;

void analyse_n_prog(n_prog *n);
void analyse_l_instr(n_l_instr *n);
void analyse_instr(n_instr *n);
void analyse_instr_si(n_instr *n);
void analyse_instr_tantque(n_instr *n);
void analyse_instr_faire(n_instr *n);
void analyse_instr_pour(n_instr *n);
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
void analyse_var(n_var *n, char *s);
void analyse_var_simple(n_var *n, char *s);
void analyse_var_indicee(n_var *n, char *s);
void analyse_appel(n_appel *n);
void analyse_tern(n_exp *n);

/**
 * @brief      Main function to parse syntaxique tree
 *
 * @param      p           The head of the tree
 * @param[in]  trace_dico  boolean to display symbol table
 * @param[in]  print_mips  boolean to display the mips output on stdin
 */
void semantique(n_prog *p, int trace_dico, int print_mips) {
  trace_tab = trace_dico;
  trace_mips = print_mips;
  analyse_n_prog(p);
}

char* prefix_string (char* s, char* p) {
  char* ss = malloc ((strlen(s) + 1) * sizeof (char));
  strcpy(ss, p);
  strcat(ss, s);
  return ss;
}

int taille_n_l_dec(n_l_dec *liste) {
  if (liste) return 1 + taille_n_l_dec(liste->queue);
  return 0;
}

int taille_n_l_exp(n_l_exp *liste) {
  if (liste) return 1 + taille_n_l_exp(liste->queue);
  return 0;
}

int newEtiquette(void) {
  return etiquette++;
}

int mips_print(const char *format, ...) {
  int done = 0;
  if (trace_mips) {
    va_list arg;
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
  }
  return done;
}

void mips_debut_fonction() {
  mips_print("\tsubi\t$sp, $sp, 4      # debut fonction\n");
  mips_print("\tsw\t$fp, 0($sp)      # debut fonction\n");
  mips_print("\tmove\t$fp, $sp         # debut fonction\n");
  mips_print("\tsubi\t$sp, $sp, 4      # debut fonction\n");
  mips_print("\tsw\t$ra, 0($sp)      # debut fonction\n");
}

void mips_fin_function() {
  mips_print("\tlw\t$ra, 0($sp)      # fin fonction\n");
  mips_print("\taddi\t$sp, $sp, 4      # fin fonction\n");
  mips_print("\tlw\t$fp, 0($sp)      # fin fonction\n");
  mips_print("\taddi\t$sp, $sp, 4      # fin fonction\n");
  mips_print("\tjr\t$ra              # fin fonction\n");
}

void mips_empile(char *s) {
  mips_print("\tsubi\t$sp, $sp, 4      # Empile\n");
  mips_print("\tsw\t$%s, 0($sp)      # Empile\n", s);
}

void mips_depile(char *s) {
  mips_print("\tlw\t$%s, 0($sp)      # Depile\n", s);
  mips_print("\taddi\t$sp, $sp, 4      # Depile\n");
}

void analyse_n_prog(n_prog *n) {
  if (n->fonctions == NULL && n->variables == NULL)
    erreur("Programme vide !");
  mips_print("\t.data\n");
  analyse_l_dec(n->variables);
  mips_print("\n\t.text\n");
  mips_print("__start:\n");
  mips_print("\tjal\tmain\n");
  mips_print("\tli\t$v0, 10\n");
  mips_print("\tsyscall          # stoppe l'execution du processus\n");
  analyse_l_dec(n->fonctions);
}

void analyse_l_instr(n_l_instr *n) {
  if (n) {
    analyse_instr(n->tete);
    analyse_l_instr(n->queue);
  }
}

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

void analyse_instr_si(n_instr *n) {
  int e1 = newEtiquette();
  int e2 = newEtiquette();
  analyse_exp(n->u.si_.test);
  mips_depile("t0");
  if(n->u.si_.sinon)
    mips_print("\tbeq\t$t0, $zero, e%d    # IF\n", e1);
  else
    mips_print("\tbeq\t$t0, $zero, e%d    # IF\n", e2);
  analyse_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    mips_print("\tj\te%d\n", e2);
    mips_print("e%d:\n", e1);
    analyse_instr(n->u.si_.sinon);
  }
  mips_print("e%d:\n", e2);
}

void analyse_instr_tantque(n_instr *n) {
  int e1 = newEtiquette();
  int e2 = newEtiquette();
  mips_print("e%d:\n", e1);
  analyse_exp(n->u.tantque_.test);
  mips_depile("t0");
  mips_print("\tbeq\t$t0, $zero, e%d    # TANTQUE\n", e2);
  analyse_instr(n->u.tantque_.faire);
  mips_print("\tj\te%d\n", e1);
  mips_print("e%d:\n", e2);
}

void analyse_instr_faire(n_instr *n) {
  int e1 = newEtiquette();
  int e2 = newEtiquette();
  mips_print("e%d:\n", e1);
  analyse_instr(n->u.faire_.faire);
  analyse_exp(n->u.faire_.test);
  mips_depile("t0");
  mips_print("\tbeq $t0, $zero, e%d    # FAIRE\n", e2);
  mips_print("\tj e%d\n", e1);
  mips_print("e%d:\n", e2);
}

void analyse_instr_pour(n_instr *n) {
  int e1 = newEtiquette();
  int e2 = newEtiquette();
  analyse_instr(n->u.pour_.init);
  mips_print("e%d:\n", e1);
  analyse_exp(n->u.pour_.test);
  mips_depile("t0");
  mips_print("\tbeq\t$t0, $zero, e%d    # POUR\n", e2);
  analyse_instr(n->u.pour_.faire);
  analyse_instr(n->u.pour_.incr);
  mips_print("\tj\te%d\n", e1);
  mips_print("e%d:\n", e2);
}

void analyse_instr_affect(n_instr *n) {
  char var[100];
  analyse_exp(n->u.affecte_.exp);
  analyse_var(n->u.affecte_.var, var);
  mips_depile("t1");
  mips_print("\tsw\t$t1, %s\n", var);
}

void analyse_instr_appel(n_instr *n) {
  analyse_appel(n->u.appel);
  if (asreturn == 0)
    mips_print("\taddi\t$sp, $sp, 4     # Valeur retour ignorée\n");
}

void analyse_appel(n_appel *n) {
  int nb_args_appel = taille_n_l_exp(n->args);
  int pos = rechercheExecutable(n->fonction);
  if (pos == -1) erreur("Fonction non déclarée !");
  if (nb_args_appel != dico.tab[pos].complement) erreur("Nombre d'arguments incorect !");
  mips_print("\tsubi\t$sp, $sp, 4     # allocation valeur de retour\n");
  analyse_l_exp(n->args);
  mips_print("\tjal\t%s\n", n->fonction);
  if (nb_args_appel > 0)
    mips_print("\taddi\t$sp, $sp, %d     # desallocation parametres\n", 4 * nb_args_appel);
}

void analyse_instr_retour(n_instr *n) {
  analyse_exp(n->u.retour_.expression);
  mips_depile("t0");
  asreturn = 1;
  mips_print("\tsw\t$t0, %d($fp)\n", 4 * (nb_args_function + 1));
  if (nb_var_local > 0)
      mips_print("\taddi\t$sp, $sp, %d      # desallocation variables locales\n", 4 * nb_var_local);
  mips_fin_function();
}

void analyse_instr_ecrire(n_instr *n) {
  analyse_exp(n->u.ecrire_.expression);
  mips_depile("a0");
  mips_print("\tli\t$v0, 1\n");
  mips_print("\tsyscall\n");
  mips_print("\tli\t$a0, '\\n'\n");
  mips_print("\tli\t$v0, 11\n");
  mips_print("\tsyscall\n");
}

void analyse_l_exp(n_l_exp *n) {
  if(n){
    analyse_exp(n->tete);
    analyse_l_exp(n->queue);
  }
}

void analyse_exp(n_exp *n) {
  if(n->type == varExp) analyse_varExp(n);
  else if(n->type == opExp) analyse_opExp(n);
  else if(n->type == intExp) analyse_intExp(n);
  else if(n->type == appelExp) analyse_appelExp(n);
  else if(n->type == lireExp) analyse_lireExp(n);
  else if(n->type == tern) analyse_tern(n);
}

void analyse_tern(n_exp *n) {
  int e1 = newEtiquette();
  int e2 = newEtiquette();
  if (n->u.tern_.test != NULL)
    analyse_exp(n->u.tern_.test);
  mips_depile("t0");
  mips_print("\tbeq $t0, $zero, e%d\n", e1);
  if (n->u.tern_.vrai != NULL)
    analyse_exp(n->u.tern_.vrai);
  mips_print("\tj\te%d\n", e2);
  mips_print("e%d:\n", e1);
  if (n->u.tern_.faux != NULL)
    analyse_exp(n->u.tern_.faux);
  mips_print("e%d:\n", e2);
}

void analyse_varExp(n_exp *n) {
  char var[100];
  analyse_var(n->u.var, var);
  mips_print("\tlw\t$t1, %s\n", var);
  mips_empile("t1");
}

void analyse_opExp(n_exp *n) {
  if(n->u.opExp_.op == plus) {
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tadd\t$t2, $t0, $t1\n");
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == moins) {
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tsub\t$t2, $t0, $t1\n");
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == fois) {
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tmult\t$t0, $t1\n");
    mips_print("\tmflo\t$t2\n");
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == divise) {
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tdiv\t$t0, $t1\n");
    mips_print("\tmflo\t$t2\n");
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == egal) {
    int e = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tli\t$t2, -1\n");
    mips_print("\tbeq\t$t0, $t1, e%d\n", e);
    mips_print("\tli\t$t2, 0\n");
    mips_print("e%d:\n", e);
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == diff) {
    int e = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t0");
    mips_depile("t1");
    mips_print("\tli\t$t2, 1\n");
    mips_print("\tbne\t$t0, $t1, e%d\n", e);
    mips_print("\tli\t$t2, 0\n");
    mips_print("e%d:\n", e);
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == inf) {
    int e = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tli\t$t2, -1\n");
    mips_print("\tblt\t$t0, $t1, e%d\n", e);
    mips_print("\tli\t$t2, 0\n");
    mips_print("e%d:\n", e);
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == infeg) {
    int e = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tli\t$t2, 1\n");
    mips_print("\tble\t$t0, $t1, e%d\n", e);
    mips_print("\tli\t$t2, 0\n");
    mips_print("e%d:\n", e);
    mips_empile("t2");
  }
  else if(n->u.opExp_.op == ou) {
    // int e1 = newEtiquette();
    // int e2 = newEtiquette();
    // if(n->u.opExp_.op1 != NULL)
    //   analyse_exp(n->u.opExp_.op1);
    // mips_depile("t0");
    // mips_print("\tbne\t$t0, $zero, e%d\n", e1);
    // if(n->u.opExp_.op2 != NULL)
    //   analyse_exp(n->u.opExp_.op2);
    // mips_depile("t0");
    // mips_print("\tbne\t$t0, $zero, e%d\n", e1);
    // mips_print("\tli\t$t1, 0\n");
    // mips_empile("t1");
    // mips_print("\tj\te%d\n", e2);
    // mips_print("e%d:\n", e1);
    // mips_print("\tli\t$t1, 1\n");
    // mips_empile("t1");
    // mips_print("e%d:\n", e2);
    int e1 = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    mips_depile("t0");
    mips_empile("t0");
    mips_print("\tbne\t$t0, $zero, e%d\n", e1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tor\t$t2, $t0, $t1\n");
    mips_empile("t2");
    mips_print("e%d:\n", e1);
  }
  else if(n->u.opExp_.op == et) {
    // int e1 = newEtiquette();
    // int e2 = newEtiquette();
    // if(n->u.opExp_.op1 != NULL)
    //   analyse_exp(n->u.opExp_.op1);
    // mips_depile("t0");
    // mips_print("\tbeq\t$t0, $zero, e%d\n", e1);
    // if(n->u.opExp_.op2 != NULL)
    //   analyse_exp(n->u.opExp_.op2);
    // mips_depile("t0");
    // mips_print("\tbeq\t$t0, $zero, e%d\n", e1);
    // mips_print("\tli\t$t1, 1\n");
    // mips_empile("t1");
    // mips_print("\tj\te%d\n", e2);
    // mips_print("e%d:\n", e1);
    // mips_print("\tli\t$t1, 0\n");
    // mips_empile("t1");
    // mips_print("e%d:\n", e2);
    int e1 = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    mips_depile("t0");
    mips_empile("t0");
    mips_print("\tbeq\t$t0, $zero, e%d\n", e1);
    if(n->u.opExp_.op2 != NULL)
      analyse_exp(n->u.opExp_.op2);
    mips_depile("t1");
    mips_depile("t0");
    mips_print("\tand\t$t2, $t0, $t1\n");
    mips_empile("t2");
    mips_print("e%d:\n", e1);
  }
  else if(n->u.opExp_.op == non) {
    int e1 = newEtiquette();
    int e2 = newEtiquette();
    if(n->u.opExp_.op1 != NULL)
      analyse_exp(n->u.opExp_.op1);
    mips_depile("t0");
    mips_print("\tbeq\t$t0, $zero, e%d\n", e1);
    mips_print("\tli\t$t1, 0\n");
    mips_print("\tj\te%d\n", e2);
    mips_print("e%d:\n", e1);
    mips_print("\tli\t$t1, 1\n");
    mips_print("e%d:\n", e2);
    mips_empile("t1");
  }
}

void analyse_intExp(n_exp *n) {
  mips_print("\tli\t$t0, %d\n", n->u.entier);
  mips_empile("t0");
}

void analyse_lireExp(n_exp *n) {
  mips_print("\tli\t$v0, 5\n");
  mips_print("\tsyscall\n");
  mips_empile("v0");
}

void analyse_appelExp(n_exp *n) {
  analyse_appel(n->u.appel);
}

void analyse_l_dec(n_l_dec *n) {
  if( n ){
    analyse_dec(n->tete);
    analyse_l_dec(n->queue);
  }
}

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

void analyse_foncDec(n_dec *n) {
  if (rechercheExecutable(n->nom) == -1) {
    mips_print("%s:\n", n->nom);
    nb_args_function = taille_n_l_dec(n->u.foncDec_.param);
    ajouteIdentificateur(n->nom, contexte, T_FONCTION, 0, nb_args_function);
    dico.base = dico.base + 1;
    entreeFonction();
    asreturn = 0;
    mips_debut_fonction();
    contexte = C_ARGUMENT;
    analyse_l_dec(n->u.foncDec_.param);
    contexte = C_VARIABLE_LOCALE;
    nb_var_local = taille_n_l_dec(n->u.foncDec_.variables);
    if (nb_var_local > 0)
      mips_print("\tsubi\t$sp, $sp, %d      # allocation variables locales\n", 4 * nb_var_local);
    analyse_l_dec(n->u.foncDec_.variables);
    analyse_instr(n->u.foncDec_.corps);
    if (trace_tab) affiche_dico();
    if (nb_var_local > 0)
      mips_print("\taddi\t$sp, $sp, %d      # desallocation variables locales\n", 4 * nb_var_local);
    mips_fin_function();
    sortieFonction();
  }
}

void analyse_varDec(n_dec *n) {
  if (contexte == C_VARIABLE_GLOBALE && rechercheExecutable(n->nom) != -1)
    erreur("Vaiable déjà déclarée !");
  if (rechercheDeclarative(n->nom) == -1) {
    if (contexte == C_VARIABLE_GLOBALE || contexte == C_VARIABLE_LOCALE) {
      ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseLocaleCourante, -1);
      if (contexte == C_VARIABLE_GLOBALE) {
        dico.base = dico.base + 1;
        mips_print("%s:\t.space\t4\n", prefix_string(n->nom, "v"));
      }
      adresseLocaleCourante += 4;
    } else if (contexte == C_ARGUMENT) {
      ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseArgumentCourant, -1);
      adresseArgumentCourant += 4;
    }
  }
}

void analyse_tabDec(n_dec *n) {
  if (rechercheDeclarative(n->nom) == -1) {
    ajouteIdentificateur(n->nom, contexte, T_TABLEAU_ENTIER, adresseLocaleCourante, n->u.tabDec_.taille);
    dico.base = dico.base + 1;
    adresseLocaleCourante += 4 * n->u.tabDec_.taille;
    mips_print("%s:\t.space\t%d\n", prefix_string(n->nom, "v"), 4 * n->u.tabDec_.taille);
  }
}

void analyse_var(n_var *n, char *s) {
  if(n->type == simple)
    analyse_var_simple(n, s);
  else if(n->type == indicee)
    analyse_var_indicee(n, s);
}

void analyse_var_simple(n_var *n, char *s) {
  int pos = rechercheExecutable(n->nom);
  if (pos == -1) erreur("Variable non déclarée !");
  if (dico.tab[pos].classe == C_VARIABLE_GLOBALE)
    sprintf(s, "%s", prefix_string(n->nom, "v"));
  else if (dico.tab[pos].classe == C_VARIABLE_LOCALE)
    sprintf(s, "%d($fp)", -1 * (8 + dico.tab[pos].adresse));
  else if (dico.tab[pos].classe == C_ARGUMENT)
    sprintf(s, "%d($fp)", 4 * nb_args_function - dico.tab[pos].adresse);
}

void analyse_var_indicee(n_var *n, char *s) {
  int pos = rechercheExecutable(n->nom);
  if (dico.tab[pos].type == T_ENTIER)
    erreur("La variable n'est pas un tableau !");
  analyse_exp(n->u.indicee_.indice);
  mips_depile("t0");
  mips_print("\tadd\t$t0, $t0, $t0\n");
  mips_print("\tadd\t$t0, $t0, $t0\n");
  sprintf(s, "%s($t0)", prefix_string(n->nom, "v"));
}
