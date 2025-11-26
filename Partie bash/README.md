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

1. **Code d'erreur 1 :**
   Si le dossier `.sh-toolbox` n'est pas présent, le script appelle `init-toolbox.sh` pour l'initialisation.

2. **Code d'erreur 2 :**
   Si le fichier `archives` n'est pas présent, il est créé et initialisé avec la valeur `0`.

3. **Code d'erreur 3 :**

   * Si une archive mentionnée dans le fichier `archives` n'existe pas, le contenu du fichier `archives` est réinitialisé avec les fichiers présents et la valeur correcte en tête de fichier.
   * Si une archive existe dans le dossier mais n'est pas mentionnée dans `archives`, le fichier `archives` est également mis à jour.

### 5. `./check-archive.sh`

**Description :**
Affiche la liste des archives .gz disponibles, permet d'en choisir une et la décompresse. Affiche ensuite l'heure de la dernière connexion suspecte et affiche la liste des fichiers modifiés après la connexion suspecte. Affiche également la liste des fichiers non modifiés qui ont le même nom que les fichiers modifiés et identifiés.

0. **Code d'erreur 0 :**
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

**Remarque :**

* La saisie utilisateur est nécessaire dans tous les cas.

**Code de retour :**

* `0` → Le script s'est exécuté correctement

---

## Conseils d'utilisation

1. Commencez par exécuter `./init-toolbox.sh` pour initialiser le dossier et le fichier `archives`.
2. Utilisez `./import-archive.sh` pour ajouter de nouvelles archives.
3. Listez vos archives avec `./ls-toolbox.sh`.
4. Exécutez `./restore-toolbox.sh` pour corriger les incohérences entre le dossier et le fichier `archives`.
