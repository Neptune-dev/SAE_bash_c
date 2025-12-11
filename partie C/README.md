# Guide de la partie C

Ce guide décrit les fichiers disponibles dans le projet C et leurs fonctionnalités.

## Conseils d'utilisation

1. Pour compiler les fichiers source, executez la commande `make`.
2. Trois executables et une librairie statiques sont créés.

<br>

# Liste des fichiers

### `makefile`

**Description :**
Permet de compiler l'ensemble des fichiers mentionnés ci-dessous

---

## /src

### 1. `cipher.c`

**Description :**
Chiffre un fichier avec une clef, selon la méthode de vignère en base64.

**Utilisation :**

`./cipher <maclef> <monfichierclair>`

---

### 2. `decipher.c`

**Description :**
Utilise `decipherbody.c` pour dechiffrer un fichier grâce à sa clef, selon la méthode de vignère en base64.

**Utilisation :**

`./decipher <maclef> <monfichierchiffre>`

---

### 3. `decipherbody.c`

**Description :**
Contient une fonciton qui dechiffre un fichier grâce à sa clef, selon la méthode de vignère en base64.

---

### 4. `findkey.c`

**Description :**
Utilise `findkeybody.c` pour trouver la clef de chiffrement d'un fichier chiffré grâce à sa version en clair.

**Utilisation :**

`./findkey <monfichierclair> <monfichierchiffre>`

---

### 5. `findkeybody.c`

**Description :**
Contient une fonction qui trouve la clef de chiffrement d'un fichier chiffré grâce à sa version en clair.


---

### 6. `tools.c`

**Description :**
Contient la table des caractère de la base64, mais aussi toutes les fonctions nécessaires au fontionnement de `cipher.c`, `decipher.c` et `findkey.c`. Bon nombre de ces fonctions sont partagées par les trois programmes, c'est pourquoi elles sont rassemblées dans ce fichier.

---

## /include

### 1. `tools.h`

**Description :**
Contient les prototypes de toutes les fonctions de `tools.c`.

---

### 2. `decipherbody.h`

**Description :**
Contient les prototypes de toutes les fonctions de `decipherbody.c`.

---

### 3. `findkeybody.h`

**Description :**
Contient les prototypes de toutes les fonctions de `findkeybody.c`.

---