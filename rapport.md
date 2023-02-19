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
- Le but de ce TP est d'implémenter un chronomètre muni de fonctions assez avancées comme minuteur, ainsi que la possibilité de compter
les tours. 

# Réponses

## Exercice 1
1. La fonction ```int intervalle_ms(struct timeval debut, struct timeval fin)``` reçois le début et la fin d'un intervalle et renvoie
la durée en milisecondes. 
2. Les quatre fonctions :
- ``` int nb_ms_vers_centiemes(int nb_ms)``` 
- ``` int nb_ms_vers_secondes(int nb_ms)```
- ``` int nb_ms_vers_minutes(int nb_ms)```
- ``` int nb_ms_vers_heures(int nb_ms)```
 
 convertissent une durée (milisecondes), en heures, minutes, secondes et centièmes de secondes.

3. 
4. 

## Exercice 2

## Exercice 3

## Exercice 4
