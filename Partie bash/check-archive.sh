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
    N=$((N+1));
done

temp_dir="$REPERTOIRE/$fichier.tmp";

if [ -d "$temp_dir" ]; then echo "Le fichier a déjà été extrait !"; exit 0; fi

# Créer un dossier temporaire pour l'extraction
mkdir -p "$temp_dir";
#si jamais on ne peut pas mkdir: erreur liée à la décompression donc même code d'erreur: 3
if [ $? -ne 0 ]; then echo "Échec lors de la création du dossier temporaire pour décompression"; exit 3; fi

# Décompression du fichier x -> extrait; z -> dezippe car .gz; f -> nom du fichier dans la commande; C -> dans le répertoire ci-contre
tar -xzf "$fichier" -C "$temp_dir";
# si échec de la commande tar
if [ $? -ne 0 ]; then echo "La décompression du fichier a échouée."; exit 3; fi

#s'il n y a pas de fichier logs
if [ ! -f "$temp_dir/var/log/auth.log" ]; then exit 4; fi
#s'il n'y a pas de dossiers dans /data
if [ $(ls $temp_dir/data| wc -w) -eq 0 ]; then exit 5; fi


# Date de référence (date de dernière connexion sur le compte admin) + conversion en timestamp pour comparaison
date_ref=$(grep -i "Accepted password for admin from" $temp_dir/var/log/auth.log | tail -n 1 | cut -d" " -f1,2,3);
ts_ref=$(date -d "$date_ref" +%s)

# tous les sous dossiers du dossier
for sous_dossier in "$temp_dir/data"/*/; do
    [ -d "$sous_dossier" ] || continue  # ignore si ce n'est pas un dossier

    # tous les fichiers du sous dossier
    for file in "$sous_dossier"*; do
        [ -f "$file" ] || continue  # ignore si ce n'est pas un fichier

        # Récupération de la date de modification
        date_modif=$(stat -c "%y" "$file" | cut -d'.' -f1)

        # Conversion en timestamp
        ts_file=$(date -d "$date_modif" +%s)

        if [ "$ts_file" -gt "$ts_ref" ]; then
            echo "fichier modifié: $file";
        elif [ "$ts_file" -lt "$ts_ref" ]; then
            #on doit reparcourir la liste des fichiers dans tous les sous dossiers afin de filtrer ceux qui sont plus anciens ou non
            #et voir s'ils ont le même nom + taille afin de les afficher
            for sous_dossier2 in "$temp_dir/data"/*/; do
                for file2 in "$sous_dossier2"*; do
                    date_modif2=$(stat -c "%y" "$file2" | cut -d'.' -f1)
                    # Conversion en timestamp
                    ts_file2=$(date -d "$date_modif2" +%s)
                    #filtre pour afficher la liste des fichiers modifiés uniquement
                    if [ "$ts_file2" -gt "$ts_ref" ]; then
                        file_name=$(basename $file);
                        size=$(ls -l $file | cut -d" " -f5);
                        file_name2=$(basename $(ls -l $file2 | cut -d" " -f9));
                        size2=$(ls -l $file2 | cut -d" " -f5);
                        if [[ $file_name = $file_name2 && $size -eq $size2 ]]; then
                            echo "pour le fichier dans la boucle: $file2, $file_name2,$size2";
                        fi
                    fi
                done
            done

        fi
    done
done
exit 0;
#rm -rf "$REPERTOIRE/$fichier.tmp";