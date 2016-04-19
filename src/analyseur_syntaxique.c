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
int trace_xml;
char nom[100];
char valeur[100];
extern int nb_ligne;
extern char yytext[];

/* ------------------------------ INTERNAL -----------------------------------*/
void EatTerminal(void) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, trace_xml);
    uniteCourante = yylex();
}

void DisplayErreur(void) {
    nom_token(uniteCourante, nom, valeur);
    fprintf(stderr, "Ligne %d erreur de syntaxe :", nb_ligne);
    fprintf(stderr, " près de l\'unité courante \"%s\"", valeur);
    fprintf(stderr, " (%d - \"%s\") \n", uniteCourante, nom);
    exit(1);
}
/* ------------------------------ INTERNAL -----------------------------------*/

n_l_dec *listeDecVariables (void) {
    n_l_dec *$$ = NULL;
    n_dec *$1 = NULL;
    n_l_dec *$2 = NULL;
    if (est_premier(uniteCourante, _declarationVariable_)) {
        affiche_balise_ouvrante("listeDecVariables", trace_xml);
        $1 = declarationVariable();
        $2 = listeDecVariablesBis();
        $$ = cree_n_l_dec($1, $2);
        affiche_balise_fermante("listeDecVariables", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_dec *listeDecFonctions (void) {
    n_l_dec *$$ = NULL;
    n_dec *$1 = NULL;
    n_l_dec *$2 = NULL;
    if (est_premier(uniteCourante, _declarationFonction_)) {
        affiche_balise_ouvrante("listeDecFonctions", trace_xml);
        $1 = declarationFonction();
        $2 = listeDecFonctions();
        $$ = cree_n_l_dec($1, $2);
        affiche_balise_fermante("listeDecFonctions", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeDecFonctions_)) {
        affiche_balise_ouvrante("listeDecFonctions", trace_xml);
        affiche_balise_fermante("listeDecFonctions", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_dec *declarationVariable (void) {
    n_dec *$$ = NULL;
    int i = -1;
    char *tmp = malloc(100*sizeof(char));
    if (uniteCourante == ENTIER) {
        affiche_balise_ouvrante("declarationVariable", trace_xml);
        EatTerminal();
        if (uniteCourante == ID_VAR) {
            EatTerminal();
            strcpy(tmp, valeur);
            i = optTailleTableau();
            if (i == -1) $$ = cree_n_dec_var(tmp);
            else if (i > 0) $$ = cree_n_dec_tab(tmp, i);
            else DisplayErreur();
            affiche_balise_fermante("declarationVariable", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_dec *declarationFonction (void) {
    n_dec *$$ = NULL;
    n_l_dec *$2 = NULL;
    n_l_dec *$3 = NULL;
    n_instr *$4 = NULL;
    char *tmp = malloc(100*sizeof(char));
    if (uniteCourante == ID_FCT) {
        affiche_balise_ouvrante("declarationFonction", trace_xml);
        EatTerminal();
        strcpy(tmp, valeur);
        $2 = listeParam();
        $3 = optDecVariables();
        $4 = instructionBloc();
        $$ = cree_n_dec_fonc(tmp, $2, $3, $4);
        affiche_balise_fermante("declarationFonction", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_dec *listeParam (void) {
    n_l_dec *$$ = NULL;
    if (uniteCourante == PARENTHESE_OUVRANTE) {
        affiche_balise_ouvrante("listeParam", trace_xml);
        EatTerminal();
        $$ = optListeDecVariables();
        if (uniteCourante == PARENTHESE_FERMANTE) {
            EatTerminal();
            affiche_balise_fermante("listeParam", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_l_instr *listeInstructions (void) {
    n_l_instr *$$ = NULL;
    n_instr *$1 = NULL;
    n_l_instr *$2 = NULL;
    if (est_premier(uniteCourante, _instruction_)) {
        affiche_balise_ouvrante("listeInstructions", trace_xml);
        $1 = instruction();
        $2 = listeInstructions();
        $$ = cree_n_l_instr($1, $2);
        affiche_balise_fermante("listeInstructions", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeInstructions_)) {
        affiche_balise_ouvrante("listeInstructions", trace_xml);
        affiche_balise_fermante("listeInstructions", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_instr *instruction (void) {
    n_instr *$$ = NULL;
    if (est_premier(uniteCourante, _instructionAffect_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionAffect();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionBloc_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionBloc();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionSi_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionSi();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionTantque_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionTantque();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionAppel_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionAppel();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionRetour_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionRetour();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionEcriture_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionEcriture();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionVide_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionVide();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _instructionPour_)) {
        affiche_balise_ouvrante("instruction", trace_xml);
        $$ = instructionPour();
        affiche_balise_fermante("instruction", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionAffect (void) {
    n_instr *$$ = NULL;
    n_var *$1 = NULL;
    n_exp *$3 = NULL;
    if (est_premier(uniteCourante, _var_)) {
        affiche_balise_ouvrante("instructionAffect", trace_xml);
        $1 = var();
        if (uniteCourante == EGAL) {
            EatTerminal();
            if (est_premier(uniteCourante, _expression_)) {
                $3 = expression();
                if (uniteCourante == POINT_VIRGULE) {
                    EatTerminal();
                    $$ = cree_n_instr_affect($1, $3);
                    affiche_balise_fermante("instructionAffect", trace_xml);
                    return $$;
                } else {
                    DisplayErreur();
                }
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionBloc (void) {
    n_instr *$$ = NULL;
    n_l_instr *$2 = NULL;
    if (uniteCourante == ACCOLADE_OUVRANTE) {
        affiche_balise_ouvrante("instructionBloc", trace_xml);
        EatTerminal();
        $2 = listeInstructions();
        if (uniteCourante == ACCOLADE_FERMANTE) {
            EatTerminal();
            $$ = cree_n_instr_bloc($2);
            affiche_balise_fermante("instructionBloc", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionSi (void) {
    n_instr *$$ = NULL;
    n_exp *$2 = NULL;
    n_instr *$4 = NULL;
    n_instr *$5 = NULL;
    if (uniteCourante == SI) {
        affiche_balise_ouvrante("instructionSi", trace_xml);
        EatTerminal();
        $2 = expression();
        if (uniteCourante == ALORS) {
            EatTerminal();
            $4 = instructionBloc();
            $5 = optSinon();
            $$ = cree_n_instr_si($2, $4, $5);
            affiche_balise_fermante("instructionSi", trace_xml);
            return $$;
        }  else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionTantque (void) {
    n_instr *$$ = NULL;
    n_exp *$2 = NULL;
    n_instr *$4 = NULL;
    if (uniteCourante == TANTQUE) {
        affiche_balise_ouvrante("instructionTantque", trace_xml);
        EatTerminal();
        $2 = expression();
        if (uniteCourante == FAIRE) {
            EatTerminal();
            $4 = instructionBloc();
            $$ = cree_n_instr_tantque($2, $4);
            affiche_balise_fermante("instructionTantque", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionAppel (void) {
    n_instr *$$ = NULL;
    n_appel *$1 = NULL;
    if (est_premier(uniteCourante, _appelFct_)) {
        affiche_balise_ouvrante("instructionAppel", trace_xml);
        $1 = appelFct();
        if (uniteCourante == POINT_VIRGULE) {
            EatTerminal();
            $$ = cree_n_instr_appel($1);
            affiche_balise_fermante("instructionAppel", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionRetour (void) {
    n_instr *$$ = NULL;
    n_exp *$2 = NULL;
    if (uniteCourante == RETOUR) {
        affiche_balise_ouvrante("instructionRetour", trace_xml);
        EatTerminal();
        $2 = expression();
        if (uniteCourante == POINT_VIRGULE) {
            EatTerminal();
            $$ = cree_n_instr_retour($2);
            affiche_balise_fermante("instructionRetour", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionEcriture (void) {
    n_instr *$$ = NULL;
    n_exp *$3 = NULL;
    if (uniteCourante == ECRIRE) {
        affiche_balise_ouvrante("instructionEcriture", trace_xml);
        EatTerminal();
        if (uniteCourante == PARENTHESE_OUVRANTE) {
            EatTerminal();
            $3 = expression();
            if (uniteCourante == PARENTHESE_FERMANTE) {
                EatTerminal();
                if (uniteCourante == POINT_VIRGULE) {
                    EatTerminal();
                    $$ = cree_n_instr_ecrire($3);
                    affiche_balise_fermante("instructionEcriture", trace_xml);
                    return $$;
                } else {
                    DisplayErreur();
                }
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionVide (void) {
    n_instr *$$ = NULL;
    if (uniteCourante == POINT_VIRGULE) {
        affiche_balise_ouvrante("instructionVide", trace_xml);
        EatTerminal();
        $$ = cree_n_instr_vide();
        affiche_balise_fermante("instructionVide", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_var *var (void) {
    n_var *$$ = NULL;
    n_exp *$2 = NULL;
    char *tmp = malloc(100*sizeof(char));
    if (uniteCourante == ID_VAR) {
        affiche_balise_ouvrante("var", trace_xml);
        EatTerminal();
        strcpy(tmp, valeur);
        $2 = optIndice();
        if ($2 != NULL)
            $$ = cree_n_var_indicee(tmp, $2);
        else if ($2 == NULL)
            $$ = cree_n_var_simple(tmp);
        affiche_balise_fermante("var", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_instr *instructionPour (void) {
    n_instr *$$ = NULL;
    n_instr *$2 = NULL;
    n_exp *$3 = NULL;
    n_instr *$5 = NULL;
    n_instr *$7 = NULL;
    if (uniteCourante == POUR) {
        affiche_balise_ouvrante("instructionPour", trace_xml);
        EatTerminal();
        $2 = instructionAffect();
        $3 = expression();
        if (uniteCourante == POINT_VIRGULE) {
            EatTerminal();
            $5 = instructionAffect();
            if (uniteCourante == FAIRE) {
                EatTerminal();
                $7 = instructionBloc();
                $$ = cree_n_instr_pour($2, $3, $5, $7);
                affiche_balise_fermante("instructionPour", trace_xml);
                return $$;
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_exp *expression (void) {
    // n_exp *$$ = NULL;
    // n_exp *$1 = NULL;
    // if (est_premier(uniteCourante, _conjonction_)) {
    //     affiche_balise_ouvrante("expression", trace_xml);
    //     $1 = conjonction();
    //     $$ = expressionBis($1);
    //     affiche_balise_fermante("expression", trace_xml);
    //     return $$;
    // }
    // DisplayErreur();
    // return $$;

    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _condition_)) {
        affiche_balise_ouvrante("expression", trace_xml);
        $1 = condition();
        $$ = conditionBis($1);
        affiche_balise_fermante("expression", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}

n_exp *condition(void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _conjonction_)) {
        affiche_balise_ouvrante("condition", trace_xml);
        $1 = conjonction();
        $$ = conditionBis($1);
        affiche_balise_fermante("condition", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}

n_exp *conditionBis (n_exp *herite) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    n_exp *$2 = NULL;
    if (uniteCourante == POINT_INTEROGATION) {
        affiche_balise_ouvrante("conditionBis", trace_xml);
        EatTerminal();
        $1 = expression();
        if (uniteCourante == DEUX_POINTS) {
            EatTerminal();
            $2 = expression();
            $$ = cree_n_exp_tern(herite, $1, $2);
            affiche_balise_fermante("conditionBis", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    } else if (est_suivant(uniteCourante, _conditionBis_)) {
        affiche_balise_ouvrante("conditionBis", trace_xml);
        affiche_balise_fermante("conditionBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}

n_appel *appelFct (void) {
    n_appel *$$ = NULL;
    n_l_exp *$2 = NULL;
    char *tmp = malloc(100*sizeof(char));
    if (uniteCourante == ID_FCT) {
        affiche_balise_ouvrante("appelFct", trace_xml);
        EatTerminal();
        strcpy(tmp, valeur);
        if (uniteCourante == PARENTHESE_OUVRANTE) {
            EatTerminal();
            $2 = listeExpressions();
            if (uniteCourante == PARENTHESE_FERMANTE) {
                EatTerminal();
                $$ = cree_n_appel(tmp, $2);
                affiche_balise_fermante("appelFct", trace_xml);
                return $$;
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_exp *conjonction (void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _negation_)) {
        affiche_balise_ouvrante("conjonction", trace_xml);
        $1 = negation();
        $$ = conjonctionBis($1);
        affiche_balise_fermante("conjonction", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *negation (void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (uniteCourante == NON) {
        affiche_balise_ouvrante("negation", trace_xml);
        EatTerminal();
        $1 = comparaison();
        $$ = cree_n_exp_op(non, $1, NULL);
        affiche_balise_fermante("negation", trace_xml);
        return $$;
    } else if (est_premier(uniteCourante, _comparaison_)) {
        affiche_balise_ouvrante("negation", trace_xml);
        $$ = comparaison();
        affiche_balise_fermante("negation", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *comparaison (void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _expArith_)) {
        affiche_balise_ouvrante("comparaison", trace_xml);
        $1 = expArith();
        $$ = comparaisonBis($1);
        affiche_balise_fermante("comparaison", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *expArith (void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _facteur_)) {
        affiche_balise_ouvrante("expArith", trace_xml);
        $1 = terme();
        $$ = expArithBis($1);
        affiche_balise_fermante("expArith", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *terme (void) {
    n_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _facteur_)) {
        affiche_balise_ouvrante("terme", trace_xml);
        $1 = facteur();
        $$ = termeBis($1);
        affiche_balise_fermante("terme", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *facteur (void) {
    n_exp *$$ = NULL;
    n_var *$1 = NULL;
    n_appel *$2 = NULL;
    if (uniteCourante == PARENTHESE_OUVRANTE) {
        affiche_balise_ouvrante("facteur", trace_xml);
        EatTerminal();
        $$ = expression();
        if (uniteCourante == PARENTHESE_FERMANTE) {
            EatTerminal();
            affiche_balise_fermante("facteur", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    } else if (uniteCourante == NOMBRE) {
        affiche_balise_ouvrante("facteur", trace_xml);
        EatTerminal();
        affiche_balise_fermante("facteur", trace_xml);
        $$ = cree_n_exp_entier( atoi(valeur));
        return $$;
    } else if (est_premier(uniteCourante, _appelFct_)) {
        affiche_balise_ouvrante("facteur", trace_xml);
        $2 = appelFct();
        affiche_balise_fermante("facteur", trace_xml);
        $$ = cree_n_exp_appel($2);
        return $$;
    } else if (est_premier(uniteCourante, _var_)) {
        affiche_balise_ouvrante("facteur", trace_xml);
        $1 = var();
        affiche_balise_fermante("facteur", trace_xml);
        $$ = cree_n_exp_var($1);
        return $$;
    } else if (uniteCourante == LIRE) {
        affiche_balise_ouvrante("facteur", trace_xml);
        EatTerminal();
        if (uniteCourante == PARENTHESE_OUVRANTE) {
            EatTerminal();
            if (uniteCourante == PARENTHESE_FERMANTE) {
                EatTerminal();
                affiche_balise_fermante("facteur", trace_xml);
                $$ = cree_n_exp_lire();
                return $$;
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    }
    DisplayErreur();
    return $$;
}
n_l_exp *listeExpressions (void) {
    n_l_exp *$$ = NULL;
    n_exp *$1 = NULL;
    if (est_premier(uniteCourante, _expression_)) {
        affiche_balise_ouvrante("listeExpressions", trace_xml);
        $1 = expression();
        $$ = cree_n_l_exp($1, listeExpressionsBis($$));
        affiche_balise_fermante("listeExpressions", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeExpressions_)) {
        affiche_balise_ouvrante("listeExpressions", trace_xml);
        affiche_balise_fermante("listeExpressions", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_exp *listeExpressionsBis (n_l_exp *herite) {
    n_l_exp *$$ = NULL;
    n_exp *$2 = NULL;
    n_l_exp *herite_fils = NULL;
    if (uniteCourante == VIRGULE) {
        affiche_balise_ouvrante("listeExpressionsBis", trace_xml);
        EatTerminal();
        $2 = expression();
        herite_fils = cree_n_l_exp($2, herite);
        $$ = listeExpressionsBis(herite_fils);
        affiche_balise_fermante("listeExpressionsBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeExpressionsBis_)) {
        affiche_balise_ouvrante("listeExpressionsBis", trace_xml);
        affiche_balise_fermante("listeExpressionsBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_prog *programme (void) {
    n_prog *$$ = NULL;
    n_l_dec *$1 = NULL;
    n_l_dec *$2 = NULL;
    affiche_balise_ouvrante("programme", trace_xml);
    if (est_premier(uniteCourante, _optDecVariables_) ||
        est_premier(uniteCourante, _listeDecFonctions_) ||
        est_suivant(uniteCourante, _programme_)) {
        $1 = optDecVariables();
        $2 = listeDecFonctions();
        affiche_balise_fermante("programme", trace_xml);
        $$ = cree_n_prog($1, $2);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *conjonctionBis (n_exp *herite) {
    n_exp *$$ = NULL;
    n_exp *$2 = NULL;
    n_exp *herite_fils = NULL;
    if (uniteCourante == ET) {
        affiche_balise_ouvrante("conjonctionBis", trace_xml);
        EatTerminal();
        $2 = negation();
        herite_fils = cree_n_exp_op(et, herite, $2);
        $$ = conjonctionBis(herite_fils);
        affiche_balise_fermante("conjonctionBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _conjonctionBis_)) {
        affiche_balise_ouvrante("conjonctionBis", trace_xml);
        affiche_balise_fermante("conjonctionBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
int optTailleTableau (void) {
    int i = -1;
    if (uniteCourante == CROCHET_OUVRANT) {
        affiche_balise_ouvrante("optTailleTableau", trace_xml);
        EatTerminal();
        if (uniteCourante == NOMBRE) {
            EatTerminal();
            i = atoi(valeur);
            if (uniteCourante == CROCHET_FERMANT) {
                EatTerminal();
                affiche_balise_fermante("optTailleTableau", trace_xml);
                return i;
            } else {
                DisplayErreur();
            }
        } else {
            DisplayErreur();
        }
    } else if (est_suivant(uniteCourante, _optTailleTableau_)) {
        affiche_balise_ouvrante("optTailleTableau", trace_xml);
        affiche_balise_fermante("optTailleTableau", trace_xml);
        return -1;
    }
    DisplayErreur();
   return -1;
}
n_exp *expArithBis (n_exp *herite) {
    n_exp *$$ = NULL;
    n_exp *$2 = NULL;
    n_exp *herite_fils = NULL;
    if (uniteCourante == PLUS) {
        affiche_balise_ouvrante("expArithBis", trace_xml);
        EatTerminal();
        $2 = terme();
        herite_fils = cree_n_exp_op(plus, herite, $2);
        $$ = expArithBis(herite_fils);
        affiche_balise_fermante("expArithBis", trace_xml);
        return $$;
    } else if (uniteCourante == MOINS) {
        affiche_balise_ouvrante("expArithBis", trace_xml);
        EatTerminal();
        $2 = terme();
        herite_fils = cree_n_exp_op(moins, herite, $2);
        $$ = expArithBis(herite_fils);
        affiche_balise_fermante("expArithBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _expArithBis_)) {
        affiche_balise_ouvrante("expArithBis", trace_xml);
        affiche_balise_fermante("expArithBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_instr *optSinon (void) {
    n_instr *$$ = NULL;
    if (uniteCourante == SINON) {
        affiche_balise_ouvrante("optSinon", trace_xml);
        EatTerminal();
        $$ = instructionBloc();
        affiche_balise_fermante("optSinon", trace_xml);
        return $$;
    }
    else if (est_suivant(uniteCourante, _optSinon_)) {
        affiche_balise_ouvrante("optSinon", trace_xml);
        affiche_balise_fermante("optSinon", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *comparaisonBis (n_exp *herite) {
    n_exp *$$ = NULL;
    n_exp *$2 = NULL;
    n_exp *herite_fils = NULL;
    if (uniteCourante == EGAL) {
        affiche_balise_ouvrante("comparaisonBis", trace_xml);
        EatTerminal();
        $2 = expArith();
        herite_fils = cree_n_exp_op(egal, herite, $2);
        $$ = comparaisonBis(herite_fils);
        affiche_balise_fermante("comparaisonBis", trace_xml);
        return $$;
    } else if (uniteCourante == INFERIEUR) {
        affiche_balise_ouvrante("comparaisonBis", trace_xml);
        EatTerminal();
        $2 = expArith();
        herite_fils = cree_n_exp_op(inf, herite, $2);
        $$ = comparaisonBis(herite_fils);
        affiche_balise_fermante("comparaisonBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _comparaisonBis_)) {
        affiche_balise_ouvrante("comparaisonBis", trace_xml);
        affiche_balise_fermante("comparaisonBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_dec *optDecVariables (void) {
    n_l_dec *$$ = NULL;
    if (est_premier(uniteCourante, _listeDecVariables_)) {
        affiche_balise_ouvrante("optDecVariables", trace_xml);
        $$ = listeDecVariables();
        if (uniteCourante == POINT_VIRGULE) {
            EatTerminal();
            affiche_balise_fermante("optDecVariables", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    } else if (est_suivant(uniteCourante, _optDecVariables_)) {
        affiche_balise_ouvrante("optDecVariables", trace_xml);
        affiche_balise_fermante("optDecVariables", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *optIndice (void) {
    n_exp *$$ = NULL;
    if (uniteCourante == CROCHET_OUVRANT) {
        affiche_balise_ouvrante("optIndice", trace_xml);
        EatTerminal();
        $$ = expression();
        if (uniteCourante == CROCHET_FERMANT) {
            EatTerminal();
            affiche_balise_fermante("optIndice", trace_xml);
            return $$;
        } else {
            DisplayErreur();
        }
    } else if (est_suivant(uniteCourante, _optIndice_)) {
        affiche_balise_ouvrante("optIndice", trace_xml);
        affiche_balise_fermante("optIndice", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_dec *listeDecVariablesBis () {
    n_l_dec *$$ = NULL;
    n_dec *$1 = NULL;
    n_l_dec *$2 = NULL;
    if (uniteCourante == VIRGULE) {
        affiche_balise_ouvrante("listeDecVariablesBis", trace_xml);
        EatTerminal();
        $1 = declarationVariable();
        $2 = listeDecVariablesBis();
        $$ = cree_n_l_dec($1, $2);
        affiche_balise_fermante("listeDecVariablesBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeDecVariablesBis_)) {
        affiche_balise_ouvrante("listeDecVariablesBis", trace_xml);
        affiche_balise_fermante("listeDecVariablesBis", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *termeBis (n_exp *herite) {
    n_exp *$2 = NULL;
    n_exp *$$ = NULL;
    n_exp *herite_fils = NULL;
    if (uniteCourante == FOIS) {
        affiche_balise_ouvrante("termeBis", trace_xml);
        EatTerminal();
        $2 = facteur();
        herite_fils = cree_n_exp_op(fois, herite, $2);
        $$ = termeBis(herite_fils);
        affiche_balise_fermante("termeBis", trace_xml);
        return $$;
    } else if (uniteCourante == DIVISE) {
        affiche_balise_ouvrante("termeBis", trace_xml);
        EatTerminal();
        $2 = facteur();
        herite_fils = cree_n_exp_op(divise, herite, $2);
        $$ = termeBis(herite_fils);
        affiche_balise_fermante("termeBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _termeBis_)) {
        affiche_balise_ouvrante("termeBis", trace_xml);
        affiche_balise_fermante("termeBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_exp *expressionBis (n_exp *herite) {
    n_exp *$2= NULL;
    n_exp *$$= NULL;
    n_exp *herite_fils= NULL;
    if (uniteCourante == OU) {
        affiche_balise_ouvrante("expressionBis", trace_xml);
        EatTerminal();
        $2 = conjonction();
        herite_fils = cree_n_exp_op(ou, herite, $2);
        $$ = expressionBis(herite_fils);
        affiche_balise_fermante("expressionBis", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _expressionBis_)) {
        affiche_balise_ouvrante("expressionBis", trace_xml);
        affiche_balise_fermante("expressionBis", trace_xml);
        $$ = herite;
        return $$;
    }
    DisplayErreur();
    return $$;
}
n_l_dec *optListeDecVariables (void) {
    n_l_dec *$$ = NULL;
    if (est_premier(uniteCourante, _listeDecVariables_)) {
        affiche_balise_ouvrante("optListeDecVariables", trace_xml);
        $$ = listeDecVariables();
        affiche_balise_fermante("optListeDecVariables", trace_xml);
        return $$;
    } else if (est_suivant(uniteCourante, _listeDecVariables_)) {
        affiche_balise_ouvrante("optListeDecVariables", trace_xml);
        affiche_balise_fermante("optListeDecVariables", trace_xml);
        return $$;
    }
    DisplayErreur();
    return $$;
}

n_prog *syntaxe(int trace_xml_tree) {
    initialise_premiers();
    initialise_suivants();
    trace_xml = trace_xml_tree;
    uniteCourante = yylex();
    return programme();
}
