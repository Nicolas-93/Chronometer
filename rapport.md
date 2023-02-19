---
title: Perfectionnement à la programmation C
subtitle: TP3 - Chronomètre
date: 13 février 2023
colorlinks: true
author:
    - Nicolas SEBAN
    - Amir POUYANFAR
geometry: margin=3cm
header-includes:
    - \usepackage{graphicx}
    - \usepackage{fancyhdr}
    # https://ctex.org/documents/packages/layout/fancyhdr.pdf
    - \pagestyle{fancy}
    - \rhead{}
    - \lhead{}
    - \renewcommand{\headrulewidth}{0pt}
    - \renewcommand{\footrulewidth}{0.2pt}
    - \lfoot{\includegraphics[height=0.8cm]{logos/namedlogoUGE.png}}
    - \rfoot{\includegraphics[height=1cm]{logos/logoLIGM.png}}
...

\pagebreak

# Introduction

- Le but de ce TP est d'implémenter un chronomètre muni de fonctions assez avancées comme un minuteur, et l'enregistrement de tours.

# Compilation

Nous avons utilisé un Makefile afin de faciliter la compilation de ce projet, vous pouvez donc utiliser, dans le racine du projet, la commande suivante afin de construire le programme :

```shell
make
```

Les binaires exécutables seront crées dans le dossier ``build``.

# Utilisation

## ChronoSimple

Le programme ``ChronoSimple`` est un chronomètre simple, il ne peut ni se mettre en pause, ni se remettre à zéro. Cette version affiche directement la durée sur la sortie standard.

Pour lancer le programme, il suffit d'exécuter la commande suivante :

```shell
./build/ChronoSimple
```

En amélioration sommaire, nous avons utilisé un retour charriot, à la place d'un retour ligne afin de ne pas écrire sur plusieurs lignes.

## ChronoMoyen

Le programme ``ChronoMoyen`` est un chronomètre un peu plus sophistiqué : il peut se mettre en pause et se remettre à zéro. Cette version utilise la bibliothèque ``ncurses``.

Pour lancer le programme, il suffit d'exécuter la commande suivante :

```shell
./build/ChronoMoyen
```

Les commandes disponibles pendant l'exécution sont les suivantes :

- La touche espace : Met le chronomètre en pause
- ``r`` : Remet le chronomètre à zéro
- ``q`` : Quitte le programme

\pagebreak

## Chronometre (version finale)

Cette version du chronomètre est la plus complète, elle permet de mettre en pause le chronomètre, de remettre à zéro le chronomètre, de lancer un minuteur, et d'enregistrer des tours.

Pour lancer le programme, il suffit d'exécuter la commande suivante :

```shell
./build/Chronometre
```

Les commandes disponibles pendant l'exécution sont les suivantes :

- La touche espace : Met le chronomètre en pause
- ``r`` : Remet le chronomètre à zéro, et supprime les tours enregistrés.
- ``t`` : Enregistre un tour
- F1-F6 ou nombre de ``1`` à ``6`` : Paramétrise l'avertissement (heures, minutes, secondes).
- Flèches haut et bas ou molette souris : Défile dans les tours enregistrés.
- ``q`` : Quitte le programme

Plusieurs améliorations ont été apportées comme la possibilité d'afficher les tours plus anciens, ou encore une colorisation de l'interface utilisateur afin de rendre l'utilisation plus agréable.
Si la fenêtre est redimensionnée, l'affichage s'adapte à la nouvelle taille, ainsi que le nombre de tours affichés, cependant si la fenêtre est trop petite, le chronomètre se fermera en affichera un message d'erreur.

# Remarques

## Exercice 1

Pour cet exercice, nous avons créé un module ``TimeUtils`` qui contient les fonctions de conversion demandées :

- ``int intervalle_ms(struct timeval debut, struct timeval fin)``{.c}
- ``int nb_ms_vers_centiemes(int nb_ms)``{.c}
- ``int nb_ms_vers_secondes(int nb_ms)``{.c}
- ``int nb_ms_vers_minutes(int nb_ms)``{.c}
- ``int nb_ms_vers_heures(int nb_ms)``{.c}

Nous y avons également ajouté une structure ``FormattedTime`` et une fonction ``ms_to_FormattedTime(time_t ms)`` renvoyant une durée formatée en plusieurs attributs : heures, minutes, secondes et centièmes de secondes.
Ce module permet d'éviter la duplication de code, avec les différentes versions de chronomètres, qui sont créés par la suite.

## Exercice 3

En cas de redimensionnement de la fenêtre, le chronomètre s'adapte à la nouvelle taille, lorsque la taille est trop petite, l'exécution s'interrompt et affiche un message d'erreur sur la sortie d'erreur standard, comme demandé dans la partie 1 (Spécifications).

Si la durée totale enregistrée est supérieure à 100 heures, le chronomètre se remet à zéro.

Les assertions demandées ont été ajoutées, elles se trouvent essentiellement dans le module ``TimeUtils``.
