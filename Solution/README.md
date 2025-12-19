# Guide de la partie Bash

Ce guide décrit les scripts disponibles dans le projet Bash et leurs fonctionnalités.

---

## Liste des scripts

### 1. `./init-toolbox.sh`

**Description :**
Crée le dossier `.sh-toolbox` ainsi que le fichier `.sh-toolbox/archives`.

**Codes de retour :**

* `0` → Fichier ou dossier créé(s) ou existant(s)
* `1` → Impossible de créer le fichier et/ou le dossier
* `2` → Le dossier contient déjà des fichiers autres que `archives`

---

### 2. `./import-archive.sh`

**Paramètres :** `file_1 file_2 ... file_n`

**Description :**
Permet d'importer une ou plusieurs archives dans le dossier `.sh-toolbox`.
Met également à jour le fichier `archives`.
Une validation utilisateur est nécessaire si une archive est déjà présente dans le dossier.

**Option :**

* `-f` → Force l'import sans demander la permission à l'utilisateur

**Codes de retour :**

* `0` → Import exécuté ou import annulé par l'utilisateur
* `1` → Le dossier `.sh-toolbox` n'existe pas
* `2` → Le chemin passé en paramètre n'est pas valide
* `3` → Erreur lors de la copie dans `.sh-toolbox`
* `4` → Problème lors de la mise à jour du fichier `archives`

---

### 3. `./ls-toolbox.sh`

**Description :**
Affiche la liste des archives présentes dans le répertoire `.sh-toolbox` en lisant le contenu du fichier `.sh-toolbox/archives`.

**Codes de retour :**

* `0` → Aucune erreur
* `1` → Le dossier `.sh-toolbox` n'existe pas
* `2` → Le fichier `archives` n'existe pas
* `3` → Une archive mentionnée dans `archives` n'existe pas dans `.sh-toolbox`
* `3` → Une archive présente dans `.sh-toolbox` n'est pas mentionnée dans `archives`

> Remarque : le code `3` peut correspondre à deux situations différentes.

---

### 4. `./restore-toolbox.sh`

**Description :**
Prend le code d'erreur retourné par `./ls-toolbox.sh` et gère les trois cas suivants :

1. **Codes de retour :**
   Si le dossier `.sh-toolbox` n'est pas présent, le script appelle `init-toolbox.sh` pour l'initialisation.

2. **Code d'erreur 2 :**
   Si le fichier `archives` n'est pas présent, il est créé et initialisé avec la valeur `0`.

3. **Code d'erreur 3 :**

   * Si une archive mentionnée dans le fichier `archives` n'existe pas, le contenu du fichier `archives` est réinitialisé avec les fichiers présents et la valeur correcte en tête de fichier.
   * Si une archive existe dans le dossier mais n'est pas mentionnée dans `archives`, le fichier `archives` est également mis à jour.

**Remarque :**

* La saisie utilisateur est nécessaire dans tous les cas.

**Code de retour :**

* `0` → Le script s'est exécuté correctement

---

### 5. `./check-archive.sh`

**Description :**
Affiche la liste des archives .gz disponibles, permet d'en choisir une et la décompresse. Affiche ensuite l'heure de la dernière connexion suspecte et affiche la liste des fichiers modifiés après la connexion suspecte. Affiche également la liste des fichiers non modifiés qui ont le même nom que les fichiers modifiés et identifiés.

0. **Code de retours :**
   Tout fonctionne.
1. **Code d'erreur 1 :**
   Si le dossier `.sh-toolbox` n'existe pas.
2. **Code d'erreur 2 :**
   Si le fichier `archives` n'existe pas.
3. **Code d'erreur 3 :**
   Si la décompression du fichier a échouée.
4. **Code d'erreur 4 :**
   Si le fichiers `log/auth.txt` est manquant.
5. **Code d'erreur 5 :**
   Si le dossier `/data/` est vide.

### 6. `restore-archive.sh`

**Paramètres :** `output_directory`


**Description:**
Dézippe l'archive choisie par l'utilisateur parmi toutes les archives disponibles dans le répertoire .sh-toolbox pour la stocker dans le répertoire `output_directory`.
Ce script détecte ensuite les paires de fichiers cryptés et clairs afin d'appeler l'exécutable `findkey` afin de pouvoir récupérer la clé et de la stocker dans `.sh-toolbox/archives`. Ce script appelle ensuite l'exécutable `decipher` afin de déchiffrer l'intégralité des fichiers et de mettre le résultat final dans un dossier `output` présent dans le répertoire courant.

0. **Code de retours :**
   Tout fonctionne.
1. **Code d'erreur 1 :**
   Si le dossier `.sh-toolbox` n'existe pas.
2. **Code d'erreur 2 :**
   Le dossier de destination n'a pas pu être créé.
3. **Code d'erreur 3 :**
   La mise à jour du fichier archives à échouée
4. **Code d'erreur 4 :**
   Si un des fichiers n'a pas pu être restaurée
   

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