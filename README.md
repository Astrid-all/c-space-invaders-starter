space# Projet final C — Space Invaders (SDL2)

## 🎯 Objectif du projet

L’objectif de ce projet est de développer un **jeu de type Space Invaders** en langage **C**, en s’appuyant sur la bibliothèque **SDL2**, à partir d’un **code de base fourni**.

Ce projet constitue **l’évaluation finale du cours de C**.  
Il vise à évaluer votre capacité à :

- comprendre et faire évoluer un code existant
- structurer un projet C réel
- gérer des structures de données, la mémoire et les états d’un programme
- utiliser correctement une bibliothèque externe
- concevoir un logiciel robuste et maintenable

--- 

## Réalisation 

- Le projet est individuel 
- La date de rendu finale est le **25/02/2026** à 12h00. 
- Le rendu doit se faire sous la forme d'un dépôt sur github 



---

## 🧱 Code de départ fourni

Le code fourni inclut :

- l’initialisation de SDL
- la création d’une fenêtre et d’un renderer
- une boucle de jeu fonctionnelle
- un vaisseau joueur affiché à l’écran
- le déplacement gauche / droite du joueur au clavier
- le joueur peut tirer **un** projectile en appuyant sur la touche espace 

Pour compiler le code de base il vous suffit de taper : 

```shell
$ make 
$ ./space_invaders 
```

Avant cela il vous faut installer sdl2. 

- macOS: `brew install sdl2`
- WSL : `sudo apt install libsdl2-dev`


---

## 🎮 Principe général du jeu

- Des **ennemis** apparaissent à l’écran et **descendent progressivement**
- Le joueur peut **tirer** pour les détruire
- Le joueur **gagne** lorsque tous les ennemis sont détruits
- Le joueur **perd** si :
  - un ennemi atteint le bas de l’écran
  - ou ses points de vie tombent à zéro

Le projet est **progressif** :  
les fonctionnalités demandées sont organisées en **niveaux de difficulté**, chacun correspondant à une **note maximale**.

---

# 🟢 Niveau 1 — Gameplay minimal (jusqu’à **10/20**)

### Fonctionnalités attendues
- Ennemis disposés en **grille**
- Descente progressive des ennemis
- Tir du joueur
- Détection de collisions :
  - tir → ennemi
- Conditions de fin :
  - victoire : tous les ennemis détruits
  - défaite : un ennemi atteint le bas de l’écran

### Contraintes techniques
- Utilisation de `struct` pour représenter :
  - le joueur
  - les ennemis
  - les tirs
- Code compilable avec : `-Wall -Wextra -Wpedantic -fanalyzer` 


### Évaluation (10 points)
| Critère | Points |
|------|------|
| Gameplay fonctionnel | 4 |
| Détection de collisions | 3 |
| Organisation du code | 2 |
| Lisibilité / propreté | 1 |
| **Total** | **10** |

---

# 🟡 Niveau 2 — Jeu structuré (jusqu’à **14/20**)

### Fonctionnalités supplémentaires
- Le joueur possède des **points de vie**
- Les ennemis peuvent **tirer**
- Affichage d’un **HUD minimal** (points de vie)
- Vitesse des ennemis légèrement progressive

### Contraintes techniques
- Séparation claire entre :
- logique de jeu
- rendu graphique
- gestion des entrées
- Libération correcte des ressources SDL
- Pas (ou très peu) de variables globales

### Évaluation cumulée
| Critère | Points |
|------|------|
| Niveau 1 validé | 10 |
| Gestion des points de vie | 2 |
| Ennemis actifs | 1 |
| Structure logicielle | 1 |
| **Total** | **14** |

---

# 🟠 Niveau 3 — Mécaniques avancées (jusqu’à **17/20**)

### Fonctionnalités supplémentaires
- Apparition aléatoire de **cœurs**
- permettent de regagner des points de vie
- **Plusieurs classes d’ennemis** :
- ennemis résistants
- ennemis rapides
- ennemis tirant plus fréquemment
- Vitesse des ennemis **progressive et paramétrable**

### Contraintes techniques
- Utilisation d’`enum` pour les types d’ennemis
- Paramétrage clair des mécaniques de jeu
- Gestion correcte des collisions multiples

### Évaluation cumulée
| Critère | Points |
|------|------|
| Niveaux 1 et 2 validés | 14 |
| Bonus (cœurs) | 1 |
| Classes d’ennemis | 1 |
| Scalabilité du code | 1 |
| **Total** | **17** |

---

# 🔴 Niveau 4 — Jeu complet (jusqu’à **20/20**)

### Fonctionnalités obligatoires
- **Système de niveaux de difficulté**
- augmentation de la vitesse
- augmentation du nombre d’ennemis
- **Gestion des états du jeu** :
- menu
- jeu
- pause
- game over
- **Sauvegarde et reprise de partie**
- sauvegarde dans un fichier
- reprise à un niveau donné

### Contraintes techniques
- Machine à états explicite (`enum GameState`)
- Sauvegarde robuste (fichier absent ou invalide géré)
- Sérialisation minimale mais cohérente des données

### Évaluation finale
| Critère | Points |
|------|------|
| Niveaux 1 à 3 validés | 17 |
| Gestion des états | 1 |
| Sauvegarde / reprise | 1 |
| Difficulté adaptative | 1 |
| **Total** | **20** |

---

## 🧪 Bonus (facultatif)

Ces éléments ne permettent pas de dépasser **20/20**, mais seront valorisés :

- Makefile propre
- Absence de fuite mémoire (Valgrind)
- Mode debug / release
- Commentaires techniques pertinents

---

## 📦 Rendu attendu

- Code source complet
- Fichiers `.h` et `.c` correctement organisés
- Makefile fonctionnel
- Instructions de compilation dans le README

---

## 🧠 Message important

> Un jeu simple, robuste et bien structuré  
> sera mieux évalué qu’un jeu complexe mais fragile.

La qualité du **code**, de la **conception** et de la **gestion mémoire**
est aussi importante que le gameplay.

---

Bon développement 🚀


