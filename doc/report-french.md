---
title: "Rapport du projet F1-of-Linux - Groupe 7"
author:
    - Melvin Campos Casares
    - Maxime De Cock
    - Dominik Fiedorczuk
    - Hubert Van De Walle
date: 3 janvier 2018
titlepage: true
---

Rapport du projet : F1-of-Linux
===============================

**Groupe *7***

Notre groupe est constitué de 4 personnes :

- Melvin Campos Casares

- Maxime De Cock

- Dominik Fiederczuk

- Hubert Van De Walle

\pagebreak

Introduction et présentation du projet
--------------------------------------

Ce projet consiste à présenter un week-end complet d'un grand prix de Formule 1
, depuis les séances d'essais du vendredi jusqu'à la course du dimanche, en 
passant par les essais du samedi et la séance de qualifications. 

Notre but consiste à reproduire cela dans un langage de programmation 
performant à l'exécution des méthodes implémentées, le langage C. 
Nous devons générer un affichage qui gèrera les séances d'essais libres, les 
qualifications ainsi que la course. 
De plus, certaines informations doivent etre disponible : temps au tour, temps 
secteur, disqualification, arret aux stands, temps depuis le début de la 
course.

De plus, nous devons appliquer des concepts vus en cours en première année 
ainsi qu'en deuxième année : processus père-fils (dont fork est la création 
d'un nouveau processus utilisateur), sémaphores (pour gérer la synchronisation 
de la mémoire ainsi que réduire l'accès aux routes partagées) et la mémoire 
partagée (allocation et utilisation par appel des mémoires partagées via leurs 
identificateurs).

\pagebreak

Cahier des charges
------------------

Il y a 20 voitures engagées dans un grand prix. 
Leurs numéros sont : 44, 77,  5,  7,  3, 33, 11, 31, 18, 35, 27, 55, 10, 28,  8
                   , 20,  2, 14,  9, 16.

Un circuit de F1 est divisé en 3 secteurs (S1, S2, S3). 

Le calendrier d’un week-end de F1 est établi comme suit : 

- Vendredi matin, une séance d’essais libres d’1h30 (P1) 

- Vendredi après-midi, une séance d’essais libres d’1h30 (P2) 

- Samedi matin, une séance d’essais libres d’1h (P3) 

- Samedi après-midi, la séance de qualifications, divisée en 3 parties : 

  - Q1, durée 18 minutes, qui élimine les 5 dernières voitures (qui occuperont 
   les places 16 à 20 sur la grille de départ de la course) 

  - Q2, durée 15 minutes, qui élimine les 6 voitures suivantes (qui occuperont 
   les places 11 à 16 sur la grille de départ de la course) 
  
  - Q3, durée 12 minutes, qui permet de classer les 10 voitures restantes pour 
   établir les 10 premières places sur la grille de départ de la course 

- Dimanche après-midi, la course en elle-même. 

Votre projet devra prendre en charge les choses suivantes. 

### Première partie : gestion des séances d’essai, qualifications et course 

#### Lors des séances d’essais (P1, P2, P3) : 

- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture 

- Toujours savoir qui a le meilleur temps dans chacun des secteurs 

- Classer les voitures en fonction de leur tour complet le plus rapide 

- Savoir si une voiture est aux stands (P) 

- Savoir si une voiture est out (abandon de la séance) 

- Dans ces 2 derniers cas, on conserve toujours le meilleur temps de la 
voiture et celle-ci reste dans le classement 

- Conserver le classement final à la fin de la séance

#### Lors des qualifications (Q1, Q2, Q3) : 

- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture 

- Toujours savoir qui a le meilleur temps dans chacun des secteurs 

- Classer les voitures en fonction de leur tour complet le plus rapide 

- Savoir si une voiture est aux stands (P) 

- Savoir si une voitures est out (abandon de la séance) 

- Dans ces 2 derniers cas, on conserve toujours le meilleur temps de la 
voiture et celle-ci reste dans le classement 

- A la fin de Q1, il reste 15 voitures qualifiées pour Q2 et les 5 dernières 
sont placées à la fin de la grille de départ (places 16 à 20) 

- A la fin de Q2, il reste 10 voitures qualifiées pour Q3 et les 5 dernières 
sont placées dans les places 11 à 15 de la grille de départ 

- Le classement de Q3 attribue les places 1 à 10 de la grille de départ 

- Conserver le classement final à la fin des 3 séances (ce sera l’ordre de 
départ pour la course) 

#### Lors de la course : 

- Le premier classement est l’ordre sur la grille de départ 

- Le classement doit toujours être maintenu tout au long de la course (gérer 
les dépassements) 

- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture 

- Toujours savoir qui a le meilleur temps dans chacun des secteurs 

- Toujours savoir qui a le tour le plus rapide 

- Savoir si la voiture est out (abandon) ; dans ce cas, elle sera classée en 
fin de classement 

- Savoir si la voiture est aux stands (PIT), gérer le temps aux stands et 
faire ressortir la voiture à sa place dans la course (généralement 2 ou 3 PIT 
par voitures) 

- Conserver le classement final et le tour le plus rapide 

**_Remarque :_ les stands se trouvent toujours dans le secteur 3.**

De plus, il vous est demandé de paramétrer votre programme. 

En effet, les circuits peuvent être de longueur très variable et, dès lors le 
nombre de tours pour la course varie également (on essaie que le nombre total 
de kilomètres soit toujours plus ou moins le même pour chacune des courses du 
calendrier). 
 
#### On vous demande de : 

- Réaliser le programme en C sous Linux 

- Utiliser la mémoire partagée comme moyen de communication inter-processus 

- Utiliser les sémaphores pour synchroniser l’accès à la mémoire partagée 

\pagebreak

Analyse du travail
------------------

Nous nous sommes réparti les taches de la manière suivante :

- Utilisation d'un système de controle de versions, nous permettant 
d'effectuer des changements chacun de notre coté (Git),

- Creation d'un makefile afin de faciliter la compilation du projet,

- Melvin a mis au clair le cahier des charges afin que nous comprenions tous 
un peu mieux les attentes pour ce projet,

- Dominik et Maxime ont implémenté une fonction permettant la génération du 
random, utilisée dans la génération du temps des secteurs ainsi que pour 
certaines probabilitées,

- Hubert a pris en charge le coté programmation lorsque les autres membres du 
groupe ne voyaient pas comment implémenter correctement les demandes du cahier 
des charges,

- Melvin et Dominik ont tenté de débugguer le projet lors du problème SIGSEGV,

- ...

\pagebreak

Difficultés rencontrées et solutions
------------------------------------

### Compréhension du cahier des charges

Au vu du cahier des charges reçu, nous avons eu des difficultés à comprendre 
plus concrètement comment mettre en oeuvre certaines implémentations demandées.
 A force de recherche et également de quelques questions posées dans le cadre 
du temps consacré spécifiquement au projet en cours, nous avons su comment 
avancer dans le projet de façon plus claire et concise malgré l'impact non 
négligeable sur la gestion du temps.

\pagebreak

### Random

Lors de l'implémentation du random, nous avons rencontré des difficultés 
concernant la génération de plusieurs valeurs différentes et dans un interval 
précis.

```C
srand(getpid());
```

Nous avons reglé ce problème en seedant le PRNG _(Pseudo Random Number 
Generator)_ avec le PID, étant unique pour chaque processus. 

### Conversion du temps

Nous avons également rencontré quelques problèmes lors de l'implémentation de 
_"timeUnit"_.

_"timeUnit"_ est une structure permettant de passer de millisecondes en tout 
autre valeur de temps réel et ce, dans les deux sens.

En effet, nos conversions n'étaient pas correctement écrites au départ, 
faisant en sorte que nos conversions ne nous rendaient des valeurs peu 
probables et arrondis; la conversion entre millisecondes et toute autre valeur 
de temps réel n'étaient pas correctement implémentée.

Après avoir reparcouru le code et avoir corrigé cela, la conversion fonctionne 
correctement.

### Sort

Nous avions rencontré un problème de segmentation et de viol d'intégrité de 
mémoire partagée suite à l'utilisation du malloc. 
En effet, malloc était trop petit, impliquant de fait le problème rencontré 
avec la mémoire partagée.

### Synchronisation

Nous avons rencontré un problème de synchronisation entre les sémaphores 
utilisé dans le projet. 
En effet, nous n'utilisions pas correctement les fonctions wait et signal des 
sémaphores, impliquant par conséquent la nécessité de débugguer le projet 
afin de détecter précisément les zones posant problème et de pouvoir les 
corriger. 

### Deadlock

Le problème de deadlock rencontré provenait des processus fils, plus 
précisément lors de la synchronisation de ces derniers. 
En débugguant le projet, nous l'avons fixé. 

### Allocation mémoire dynamique

En utilisant une longueur variable pour le circuit, le nombre de tours varie 
et il faut donc allouer les tableaux contenant la durée des différents secteurs
 de manière dynamique. 
Par manque de temps, nous l'avons pas implémenté. 

\pagebreak

Evolutions futures
------------------

### Un meilleur affichage

L'affichage actuellement présent dans le projet n'est pas exactement celui 
demandé d'après le cahier des charges. 
Une réimplémentation de l'affichage pour contenir toutes les informations 
demandées avec, éventuellement, un jeu de couleurs pourrait s'avérer 
intéressant. 

Par exemple, nous pourrions implémenter un affichage ressemblant à ceci : 

Afficher en Temps tour si P (P se trouve en S3) ou OUT (si abandon ou crash). 
Dans le cas de OUT, la voiture est dernière du classement. 

Les codes d'affichage pour les tableaux sont : 

|    Code     |    S    |       P       |   OUT   |
|-------------|---------|---------------|---------|
| Description | Secteur | Stand (= PIT) | Abandon |

#### Pour les essais libres :

_**Tri temps meilleur tour**_

| Numéro de la voiture | S1 | S2 | S3 | Temps meilleur tour |
|----------------------|----|----|----|---------------------|
| 2                    | 35 | 39 | 38 | 1min 52s            |
| 55                   | 40 | 36 | 39 | 1min 55s            |
| 14                   | 39 | 40 | 37 | 1min 56s            |
| 28                   | 37 | 40 | 39 | 1min 56s            |

#### Pour les qualifications :

_**Tri temps meilleur tour**_

| Numéro de la voiture | S1 | S2 | S3 | Temps tour | Temps meilleur tour |
|----------------------|----|----|----|------------|---------------------|
| 14                   | 39 | 40 | 37 | 1min 56s   | 1min 32s            |
| 2                    | 35 | 39 | 38 | 1min 52s   | 1min 47s            |
| 55                   | 40 | 36 | 39 | 1min 55s   | 1min 55s            |
| 28                   | 39 | 40 | 37 | 1min 56s   | 1min 39s            |

#### Pour la course :

_**Tri position dans la course**_

| Numéro de la voiture | S1 | S2 | S3 | Temps tour | Lap |
|----------------------|----|----|----|------------|-----|
| 2                    | 35 | 39 | 38 | 1min 52s   | 14  |
| 55                   | 40 | 36 | 39 | 1min 55s   | 14  |
| 14                   | 39 | 40 | 37 | 1min 56s   | 14  |
| 28                   | 39 | 40 | 37 | 1min 56s   | 14  |

\pagebreak

Conclusion
----------

Ce projet nous a permis d'apprendre à programmer de façon plus assidue. Lors 
de l'écriture d'une nouvelle méthode, nous testions systématiquement le projet 
et en cas de problème, nous prenions le temps de relire le code et si 
nécessaire, nous testions différentes méthodes (notamment le classique 
_"print"_) pour débugguer notre méthode et avancer dans le projet. 
Nous avons pu appliquer plusieurs concepts vu en cours théorique au courant du 
quadrimestre et comprendre plus concrètement ce que ces concepts permettent de 
faire (allocation d'une zone mémoire, appel d'une zone mémoire, sémaphores, 
algorithmes, fork, etc.).

Au début de ce projet, nous avions rencontré quelques difficultés de 
compréhension par rapport au cahier des charges, ce qui nous a pris un peu de 
temps au début et, par conséquent, un retard par rapport au planning que nous 
avons pu rattraper en courant d'année. 

\pagebreak

Exemplaire du code
------------------

```C
\\TODO
```
