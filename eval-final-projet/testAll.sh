#!/bin/bash
##############################################################################
#
# Script d'évaluation automatique du compilateur.
# Compile le programme source et compare la sortie avec la référence.
# Pour le code MIPS, compare la SORTIE de l'exécution sur Mars
# Comprend 
#   * analyse lexicale, 
#   * analyse syntaxique, 
#   * arbre abstrait, 
#   * table des symboles et
#   * code MIPS.
#
# ATTENTION : pour que le script marche, votre compilateur doit returner 0 em
# cas de succès, autre valeur en cas d'erreur (entrée ne compile pas 
# correctement) et doit permettre d'afficher les sorties sur stdout, sauf 
# éventuels messages d'erreur, sur stderr. On doit pouvoir, à tout moment,
# changer le type de sortie (lex, synt, asynt, tab, mips) avec des options.
#
##############################################################################
###     LLL      IIIIIII    RRRRRRR     EEEEEEEEE     ||||
###     LLL        III      RRR  RRR    EEEEEEEEE     ||||
###     LLL        III      RRR    RR   EEE           ||||
###     LLL        III      RRR  RRR    EEEEEEEE      ||||
###     LLL        III      RRRRRRR     EEEEEEEE    \\\\//// 
###     LLL        III      RRR RRR     EEE          \\\/// 
###     LLLLLLLL   III      RRR   RRR   EEEEEEEE      \\// 
###     LLLLLLLL IIIIIII    RRR   RRR   EEEEEEEEE      \/  
##############################################################################

# 1) MODIFIEZ LA VARIABLE CI-DESSOUS AVEC LE CHEMIN VERS VOTRE COMPILATEUR

MYCOMPILO="../compilateur"

# 2) DÉCOMMENTEZ + MODIFIEZ LES COMMANDES POUR GÉNÉRER LES DIFFÉRENTES SORTIES

MYCOMPILODEFAULT="${MYCOMPILO}"  # utilisé pour test reconnaissance et erreur
#MYCOMPILOLEX="${MYCOMPILO} -l"   # exécuter l'analyseur lexical
#MYCOMPILOSYNT="${MYCOMPILO} -s"  # exécuter l'analyseur syntaxique
MYCOMPILOASYNT="${MYCOMPILO} -a" # afficher l'arbre abstrait
MYCOMPILOTAB="${MYCOMPILO} -t"   # afficher les tables des symboles
MYCOMPILOMIPS="${MYCOMPILO} -m"  # générer code MIPS
EXITONFAIL=1                     # mettre à zéro pour continuer après erreurs
MARS="./Mars4_5.jar"             # utilisez autre version de Mars si besoin
##############################################################################
# NE PLUS LIRE À PARTIR D'ICI ;-)
##############################################################################

declare -A testname
testname["lex"]="Analyse lexicale"
testname["synt"]="Analyse syntaxique"
testname["asynt"]="Arbre abstrait"
testname["tab"]="Table des symboles"
testname["mips"]="Code machine MIPS"

##############################################################################
# Fonction pour faire le diff textuel régulier (lex et tab).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier 1
# @param $2 Fichier 2
# @return 0 si pas de différence, autre sinon

function REGDIFF() {
  diff -q -w $1 $2
}

##############################################################################
# Fonction pour faire le diff entre deux fichiers XML (syn et asyn).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier XML 1
# @param $2 Fichier XML 2
# @return 0 si pas de différence, autre sinon

function XMLDIFF() {
  ./compare_arbres_xml $1 $2
}

##############################################################################
# Fonction pour faire le diff entre deux sorties d'exécution Mars (mips).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier mips 1
# @param $2 Fichier mips 2
# @param $3 Entrée à donner à l'exécution du programme (optionnel)
# @return 0 si pas de différence, autre sinon

function MARSOUTDIFF() {
  diff <(echo -e "$3" | java -jar $MARS $1 | tail -n +2 | sed ':a;N;$!ba;s/[ \n]//g') <(echo -e "$3" | java -jar $MARS $2 | tail -n +2 | sed ':a;N;$!ba;s/[ \n]//g')
}

##############################################################################
# Fonction pour faire le diff entre deux fichiers, vérifier et afficher le 
# résultat avec de belles couleurs.
# @param $1 Nom de la fonction utilisée pour faire le diff (parmi 3 ci-dessus)
# @param $2 Nom du fichier d'entrée à tester sans extension (p. ex. affect)
# @param $3 Extension du fichier à tester (p. ex. synt)
# @param $4 Entrée à donner à l'exécution du programme (optionnel)

function diff_prog() {
  diffprog=$1
  input=$2
  suffix=$3
  values=$4
  refoutput="ref-${suffix}/${input}.${suffix}"
  sysoutput="output/${input}.${suffix}"
  echo -e "\033[35m > ${testname[${suffix}]} (.${suffix})\033[0m"
  echo -e "`wc -l output/${input}.${suffix} | awk '{print $1}'` lignes"
  if [ -f ref-${suffix}/$input.${suffix} ]; then
    ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix} ${values} 2>&1 2> /dev/null
    if [ $? != 0 ]; then 
      echo -e "\033[31mTEST ${testname[${suffix}]} ÉCHOUÉ\033[0m"
      echo -e "Différences entre output/${input}.${suffix} ref-${suffix}/${input}.${suffix} en utilisant $diffprog:"
      ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix} ${values}
      if [ $EXITONFAIL = 1 ]; then exit 1; fi
    else
      echo -e "\033[32mTEST ${testname[${suffix}]} OK\033[0m"
    fi
  else
    echo -e "\033[34mRéférence ref-${suffix}/${input}.${suffix} absente\033[0m"
  fi
}

##############################################################################
# Exécute tous les tests (reco, lex, synt, asynt, tab, mips) pour un exemple
# donné qui doit être correctement compilé à tous les niveaux.
# @param $1 Nom du fichier d'entrée à tester sans extension (p. ex. affect)
# @param $2 Entrée à donner à l'exécution du programme (optionnel)

function test_fichier_ok() {
    input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    if [ -n "$2" ]; then
      echo "Input : $2"
    fi
    if [ -f input/$input.l ]; then
      # Reconnaissance : programme correct doit être accepté
      if [ -n "${MYCOMPILODEFAULT}" ]; then
        echo -e "\033[35m > Reconnaissance (accepte l'entrée)\033[0m"        
        ${MYCOMPILODEFAULT} input/$input.l > output/$input.synt
        if [ $? != 0 ]; then 
          echo -e "\033[31mTEST Reconnaissance ÉCHOUÉ\033[0m"
          echo -e "Le programme $input.l n'a pas été compilé correctement"	
          if [ $EXITONFAIL = 1 ]; then exit 1; fi
        else
          echo -e "\033[32mTEST Reconnaissance OK\033[0m"
        fi
      fi     
      # Teste analyseur lexical
      if [ -n "${MYCOMPILOLEX}" ]; then
        ${MYCOMPILOLEX} input/$input.l > output/$input.lex
        diff_prog REGDIFF $input lex
      fi 
      # Teste analyseur syntaxique
      if [ -n "${MYCOMPILOSYNT}" ]; then
        ${MYCOMPILOSYNT} input/$input.l > output/$input.synt          
        diff_prog XMLDIFF $input synt
      fi
      # Teste création de l'arbre abstrait
      if [ -n "${MYCOMPILOASYNT}" ]; then
        ${MYCOMPILOASYNT} input/$input.l > output/$input.asynt
        diff_prog XMLDIFF $input asynt
      fi
      # Teste replissage de la table des symboles
      if [ -n "${MYCOMPILOTAB}" ]; then
        ${MYCOMPILOTAB} input/$input.l > output/$input.tab
        diff_prog REGDIFF $input tab
      fi
      # Teste génération de code MIPS
      if [ -n "${MYCOMPILOMIPS}" ]; then
        ${MYCOMPILOMIPS} input/$input.l > output/$input.mips
        diff_prog MARSOUTDIFF $input mips "$2"
      fi
    else
      echo -e "\033[31minput/$input.l non trouvé\033[0m"
      echo -e "\033[31mTest impossible\033[0m"        
    fi
}

##############################################################################
# Vérifie qu'un programme contenant des erreurs n'est pas reconnu/compilé.
# @param $1 Nom du fichier d'entrée à tester sans extension (p. ex. affect)

function test_fichier_fail() {
	input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    ${MYCOMPILODEFAULT} input/$input.l > output/$input.synt.xml
    if [ $? = 0 ]; then 
    echo -e "\033[31mTEST REJET ÉCHOUÉ\033[0m"
      echo -e "Le programme $input.l est accepté mais il devrait être rejeté"
      if [ $EXITONFAIL = 1 ]; then exit 1; fi
    else
      echo -e "\033[32mTEST REJET OK\033[0m"
    fi
}

##############################################################################
#######  #####  ######   ######    ###  #####  ###############################
#######   ###   #####  #  ######  ####    ###  ###############################
#######    #    ####  ###  #####  ####  ##  #  ###############################
#######  #   #  ###         ####  ####  ###    ###############################
#######  ## ##  ##  #######  ##    ###  ####   ###############################
##############################################################################

mkdir -p output
make clean
make

################################################################################
# Vérifications initiales : MYCOMPILO et MARS bien configurés?

echo -e "Votre compilateur : ${MYCOMPILO}"
if [ ! -f  ${MYCOMPILO} ]; then
  echo -e "\033[31mCompilateur introuvable"
  echo -e "Modifiez la variable MYCOMPILO avant de lancer l'éval\033[0m"
  exit 1
fi
if [ -n "${MYCOMPILOMIPS}" -a ! -f  ${MARS} ]; then
  echo -e "\033[31mMARS introuvable"
  echo -e "Modifiez la variable MARS avant de lancer l'éval\033[0m"
  exit 1
fi

################################################################################

echo -e "\033[1m\n>> 1.1) Tests connus OK\033[0m"

test_fichier_ok eval1
test_fichier_ok eval2
test_fichier_ok eval3
# 3 cas de figure selon les entrées tapées
test_fichier_ok eval4 "3\n10\n"
test_fichier_ok eval4 "10\n10\n"
test_fichier_ok eval4 "-3\n-10\n"
test_fichier_ok eval5

################################################################################

echo -e "\033[1m\n>> 1.2) Tests connus FAIL\033[0m"

test_fichier_fail eval1-err

################################################################################

echo -e "\033[1m\n>> 2.1) Tests nouveaux OK\033[0m"

test_fichier_ok eval6
test_fichier_ok eval7
test_fichier_ok eval8
test_fichier_ok eval9
test_fichier_ok eval10
test_fichier_ok eval11
test_fichier_ok eval12
test_fichier_ok eval13
test_fichier_ok eval14
test_fichier_ok eval15
test_fichier_ok eval16
test_fichier_ok eval17
test_fichier_ok eval18
test_fichier_ok eval19
test_fichier_ok eval20 "0"
test_fichier_ok eval20 "1"
test_fichier_ok eval20 "10"
test_fichier_ok eval20 "16"
test_fichier_ok eval21 "-1"
test_fichier_ok eval21 "1"
test_fichier_ok eval21 "10"
test_fichier_ok eval21 "16"
test_fichier_ok eval22 "10\n5\n"
test_fichier_ok eval22 "31\n175\n"
test_fichier_ok eval22 "2\n2\n"
test_fichier_ok eval23 "1"
test_fichier_ok eval23 "0"
test_fichier_ok eval24 "7\n7\n7\n7\n"
test_fichier_ok eval24 "0\n7\n0\n7\n"
test_fichier_ok eval24 "7\n0\n7\n0\n"
test_fichier_ok eval24 "0\n0\n0\n0\n"

################################################################################

echo -e "\033[1m\n>> 2.2) Tests nouveaux FAIL\033[0m"

test_fichier_fail eval2-err
test_fichier_fail eval3-err
test_fichier_fail eval4-err
test_fichier_fail eval5-err
test_fichier_fail eval6-err
test_fichier_fail eval7-err
test_fichier_fail eval8-err
test_fichier_fail eval9-err
test_fichier_fail eval10-err
test_fichier_fail eval11-err
test_fichier_fail eval12-err

################################################################################

echo -e "\033[1m\n>> 3.1) Tests nouvelle fonctionnalité OK\033[0m"

test_fichier_ok eval25 "9"
test_fichier_ok eval25 "10"
test_fichier_ok eval25 "11"
test_fichier_ok eval26 "-50"
test_fichier_ok eval26 "50"
test_fichier_ok eval26 "150"
test_fichier_ok eval27

################################################################################
echo -e "\033[1m\n>> 3.2) Tests nouvelle fonctionnalité FAIL\033[0m"

test_fichier_fail eval13-err
