#!/bin/bash

REPERTOIRE=".sh-toolbox"

if [ ! -d $REPERTOIRE ]; then echo "Le répertoire $REPERTOIRE n'existe pas."; exit 1; fi
if [ ! -f "$REPERTOIRE/archives" ]; then echo "Le fichier $REPERTOIRE/archives n'existe pas."; exit 2; fi

echo "Liste des fichiers .gz dans $REPERTOIRE :"

N=1
TOTAL=0

#boucle d'affichage des fichiers .gz dans le répertoire
for file in "$REPERTOIRE"/*.gz; do
    base_file=$(basename "$file")
    echo "$N: $base_file"
    N=$((N+1))
done

TOTAL=$N

# Vérifier s'il y a au moins un fichier
if [ "$TOTAL" -eq 1 ]; then echo "Aucun fichier .gz trouvé dans $REPERTOIRE."; exit 1; fi

# tant que la valeur saisie est pas valide
while true; do
    echo -n "Entrez le numéro du fichier à sélectionner : "
    read CHOIX

    # si c'est pas un nombre -> on reste dans la boucle
    if ! [[ "$CHOIX" =~ ^[0-9]+$ ]]; then echo "Erreur : vous devez entrer un nombre."; continue; fi

    # intervalle de la variable choix non compris entre 1 et le nombre de fichiers .gz au total -> on reste dans la boucle
    if (( CHOIX < 1 || CHOIX >= TOTAL )); then echo "Erreur : numéro invalide."; continue; fi

    # Si tout est bon, on sort de la boucle
    break
done

# variable CHOIX qui contient le bon chiffre, donc maintenant on reparcourt le répertoire afin de pouvoir stocker le nom du fichier
fichier=""
N=1
for file in "$REPERTOIRE"/*.gz; do
    base_file=$(basename "$file")

    if [ "$N" -eq "$CHOIX" ]; then
        fichier="$base_file"
        echo "Vous avez sélectionné : $fichier"
        break
    fi
    N=$((N+1))
done

# Créer un dossier temporaire pour l'extraction
mkdir -p "$REPERTOIRE/$fichier.tmp"
cp "$REPERTOIRE/$fichier" "$REPERTOIRE/$fichier.tmp"
#la copie est nécéssaire pour la décompression donc on inclut cette copie dans le cas où la décompression échoue
if [ $? -eq 0 ]; then echo "Échec lors de la copie du fichier pour décompression"; exit 3; fi

# Décompression du fichier x -> extrait; z -> dezippe car .gz; f -> nom du fichier dans la commande; C -> dans le répertoire ci-contre
tar -xzf "$REPERTOIRE/$fichier.tmp/$fichier" -C "$REPERTOIRE/$fichier.tmp"
# si échec de la commande tar
if [ $? -eq 0 ]; then echo "La décompression du fichier a échouée."; exit 3; fi
