# Guide de la partie C

Ce guide décrit les fichiers disponibles dans le projet C et leurs fonctionnalités.

---

## Liste des fichiers sources

### 1. `cipher.c`

**Description :**
Chiffre un fichier avec une clef, selon la méthode de vignère en base64.

**Syntaxe :**

`./cipher maclef monfichierclair`

---

### 2. `decipher.c`

**Description :**
Dechiffre un fichier grâce à sa clef, selon la méthode de vignère en base64.

**Syntaxe :**

`./decipher maclef monfichierchiffre`

---

### 3. `findkey.c`

**Description :**
Trouve la clef de chiffrement d'un fichier chiffré grâce à sa version en clair.

**Syntaxe :**

`./findkey monfichierclair monfichierchiffre`

---

### 4. `tools.c`

**Description :**
Contient la table des caractère de la base64, mais aussi toutes les fonctions nécessaires au fontionnement de `cipher.c`, `decipher.c` et `findkey.c`. Bon nombre de ces fonctions sont partagées par les trois programmes, c'est pourquoi elles sont rassemblées dans ce fichier.

---

### 5. `tools.h`

**Description :**
Contient les prototypes de toutes les fonctions de `tools.c`.

---

### 6. `makefile`

**Description :**
Permet de compiler l'ensemble des fichiers mentionnés ci-dessus en trois executables :
* `cipher`
* `decipher`
* `findkey`

---


## Conseils d'utilisation

1. Pour compiler les fichiers source, rendez-vous dans le répertoire `src/` et executez la commande `make`.
2. Vous pouvez utiliser à votre guise les executables.