# Guide de la partie C

Ce guide décrit les fichiers disponibles dans le projet C et leurs fonctionnalités.

## Conseils d'utilisation

1. Pour compiler les fichiers source, exécutez la commande `make`
2. Trois exécutables et une librairie statique sont créés.

* Utilisation des exécutables seuls : voir la liste des fichiers pour :
    * `cipher.c`
    * `decipher.c`
    * `findkey.c`
* Utilisation de la biliothèque statique :
    * inclure les fonctions désirées de la bibliothèque :
        ```C
        #include "decipherbody.h"
        #include "findkeybody.h"
        ```
    * compiler avec `gcc <mon_programme.c>  -L. -lsae -o <mon_programme>`
    * fonctions de la bibliothèque :
        ```C
        // dechiffre un fichier grâce à la clef, et exporte le resultat dans un fichier
        Decipher(char* clefclair, char* fichierchiffree);

        /* trouve la clef en base64, l'affiche sur la sortie standart
           la taille de la clef est donnée sur la sortie d'erreur      */
        FindKey(char* fichierclair, char* fichierchiffree);
        ```

<br>

# Liste des fichiers

### `makefile`

**Description :**
Permet de compiler l'ensemble des fichiers mentionnés ci-dessous

---

## /src

Ce repertoire contient tous les fichier `.c`, possédant chacun une fonction de nettoyage de la mémoire ainsi que les fonctions et programmes principaux de la solution.

---

### 1. `cipher.c`

**Description :**
Chiffre un fichier avec une clef, selon la méthode de Vigenère en base64.

**Utilisation :**

`./cipher <maclef> <monfichierclair>`

Pour avoir le résultat décodé, il faudra ensuite passer par la commande Bash 

`base64 -d <monfichierencodé> > <monfichierdécodé>`

---

### 2. `decipher.c`

**Description :**
Utilise `decipherbody.c` pour déchiffrer un fichier grâce à sa clef, selon la méthode de Vigenère en base64.

**Utilisation :**

`./decipher <maclef> <monfichierchiffré>`

Pour avoir le résultat décodé, il faudra ensuite passer par la commande Bash 

`base64 -d <monfichierencodé> > <monfichierdécodé>`

---

### 3. `decipherbody.c`

**Description :**
Contient une fonction qui déchiffre un fichier grâce à sa clef, selon la méthode de Vigenère en base64.

---

### 4. `findkey.c`

**Description :**
Utilise `findkeybody.c` pour trouver la clef de chiffrement d'un fichier chiffré grâce à sa version en clair.

**Utilisation :**

`./findkey <monfichierclair> <monfichierchiffre>`

---

### 5. `findkeybody.c`

**Description :**
Contient une fonction qui trouve la clef de chiffrement d'un fichier chiffré grâce à sa version en clair. La clef encodée en Base64 est donnée sur la sortie standard tandis que la taille de la clef est donnée sur la sortie d'erreur.

---

### 6. `tools.c`

**Description :**
Contient la table des caractères de la base64, mais aussi toutes les fonctions nécessaires au fontionnement de `cipher.c`, `decipher.c` et `findkey.c`. Bon nombre de ces fonctions sont partagées par les trois programmes, c'est pourquoi elles sont rassemblées dans ce fichier.

---

## /include

Ce repertoire contient tous les fichier `.h`, les prototypes des fichiers de `/src`

--

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