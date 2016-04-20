Projet de Compilation
=====================

auteurs : ALLAM Thomas & BERNARDINI Mickaël

#Sommaire
* Introduction
* Sructure du pojet
* Compilation
* Execution
* Note

#Introduction

Le but de ce projet a été de réaliser le compilateur du langage L, langage de programmation simplifié inspiré du C.

#Sructure du pojet

**src :**
Répertoire contenant le code source du projet, dont le fichier contenant la fonction main : compilateur.c

**inc :**
Répertoire contenant les fichiers include, les headers du projet.

**test :**
Répertoire contenant tout les fichiers d'entrée pour le compilateur (fichier de test) ainsi que les différentes références de résultat attendu.

#Compilation

Le projet comporte un MAKEFILE qui facilite la compilation en utilisant la commande *make*.
On peut également utiliser la commande *make buildAndRun* afin de compilé et d'executer automatiquement le projet avec le script de lévaluation finale.
La compilation génère le fichier executable *compilateur*.

#Execution

Pour executer le projet il suffit d'executer le fichier *compilateur* suivi de l'option désirée puis du fichier L à compiler :

	./compilateur OPTION FILE

##Options

**-l :**
Permet d'afficher la sortie de l'analyseur lexical.

**-s :**
Permet d'afficher l'arbre syntaxique.

**-a :**
Permet d'afficher l'arbre abstrait

**-t :**
Permet d'afficher la sortie de l'analyseur sémantique.

**-m :**
Permet d'afficher le code mips généré. 

#Note

Durant l'évaluation du projet, le mardi 19 avril 2016, un bug de la nouvelle fonctionnalité subsistait dans l'arbre syntaxique. Le bug est maintenant corrigé et notre compilateur passe tout les tests de lévaluation.