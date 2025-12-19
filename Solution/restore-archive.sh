#!/bin/bash

REPERTOIRE=".sh-toolbox"

# 1. Vérifier les arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <destination-directory>"
    exit 1
fi

DEST_DIR="$1"

# Vérifier que .sh-toolbox existe
if [ ! -d "$REPERTOIRE" ]; then
    echo "Le répertoire $REPERTOIRE n'existe pas."
    exit 1
fi

if [ ! -f "$REPERTOIRE/archives" ]; then
    echo "Le fichier $REPERTOIRE/archives n'existe pas."
    exit 1
fi

# Créer le dossier de destination s'il n'existe pas
if [ ! -d "$DEST_DIR" ]; then
    mkdir -p "$DEST_DIR"
    if [ $? -ne 0 ]; then
        echo "Erreur: Impossible de créer le dossier de destination '$DEST_DIR'."
        exit 2
    fi
fi

# 2. Demander à l'utilisateur quelle archive restaurer
echo "Liste des archives disponibles :"
N=1
TOTAL=0

if ! ls "$REPERTOIRE"/*.gz >/dev/null 2>&1; then
    echo "Aucune archive trouvée."
    exit 1
fi


for file in "$REPERTOIRE"/*.gz; do
    base_file=$(basename "$file")
    echo "$N: $base_file"
    N=$((N+1))
done

TOTAL=$N

# Sélectionner l'archive à restaurer
while true; do
    echo -n "Entrez le numéro de l'archive à restaurer : "
    read CHOIX

    if ! [[ "$CHOIX" =~ ^[0-9]+$ ]]; then
        echo "Erreur : vous devez entrer un nombre."
        continue
    fi

    if (( CHOIX < 1 || CHOIX >= TOTAL )); then
        echo "Erreur : numéro invalide."
        continue
    fi

    break
done

# Récupérer le nom de l'archive sélectionnée
fichier=""
N=1
for file in "$REPERTOIRE"/*.gz; do
    base_file=$(basename "$file")
    if [ "$N" -eq "$CHOIX" ]; then
        fichier="$base_file"
        echo "Archive sélectionnée : $fichier"
        break
    fi
    N=$((N+1))
done
output_dir="$DEST_DIR";

# Créer un dossier temporaire pour l'extraction
mkdir -p "$output_dir";
#si jamais on ne peut pas mkdir: erreur liée à la décompression donc même code d'erreur: 3
if [ $? -ne 0 ]; then echo "Échec lors de la création du dossier temporaire pour décompression"; exit 3; fi

# Décompression du fichier x -> extrait; z -> dezippe car .gz; f -> nom du fichier dans la commande; C -> dans le répertoire ci-contre
tar -xzf "$fichier" -C "$output_dir";
# si échec de la commande tar
if [ $? -ne 0 ]; then echo "La décompression du fichier a échouée."; exit 3; fi

#s'il n y a pas de fichier logs
if [ ! -f "$output_dir/var/log/auth.log" ]; then exit 4; fi
#s'il n'y a pas de dossiers dans /data
if [ $(ls $output_dir/data| wc -w) -eq 0 ]; then exit 5; fi


# Date de référence (date de dernière connexion sur le compte admin) + conversion en timestamp pour comparaison
date_ref=$(grep -i "Accepted password for admin from" $output_dir/var/log/auth.log | tail -n 1 | cut -d" " -f1,2,3);
ts_ref=$(date -d "$date_ref" +%s);

# Afficher les fichiers créés après la date de référence dans chaque dossier de /data
find "$output_dir/data" -type f | while read -r file_modifie; do
    mtime_file=$(stat -c %Y "$file_modifie" 2>/dev/null)
    
    # Vérifier si le fichier est modifié
    if [ "$mtime_file" -ge "$ts_ref" ]; then
        name_modifie=$(basename "$file_modifie")
        size_modifie=$(stat -c %s "$file_modifie" 2>/dev/null)
        
        # on utilise find pour parcourir à nouveau tous les fichiers et trouver les non modifiés
        find "$output_dir/data" -type f | while read -r file_non_modifie; do
            mtime_file2=$(stat -c %Y "$file_non_modifie" 2>/dev/null)
            
            # Vérifier si le fichier est non modifié
            if [ "$mtime_file2" -lt "$ts_ref" ]; then
                name_non_modifie=$(basename "$file_non_modifie")
                size_non_modifie=$(stat -c %s "$file_non_modifie" 2>/dev/null)
                
                # Comparer nom et taille
                if [ "$name_modifie" = "$name_non_modifie" ] && [ "$size_modifie" -eq "$size_non_modifie" ]; then
                    relative_modifie="${file_modifie#$output_dir/}"
                    relative_non_modifie="${file_non_modifie#$output_dir/}"
                    echo "Paire trouvée:"
                    echo "  Modifié:     $relative_modifie"
                    echo "  Non modifié: $relative_non_modifie"
                    
                    # findkey pour retrouver la clé et base64 -d pour décoder la clé et l'afficher
                    key=$(./findkey "$file_non_modifie" "$file_modifie" 2>/dev/null);
                    key_decoded=$(echo "$key" | base64 -d 2>/dev/null);
                    echo "  Clé: $key_decoded"

                    # decipher pour récupérer le contenu
                    ./decipher "$key_decoded" "$file_modifie" 2>/dev/null;
                    mkdir output;
                    cat "deciphered_output.txt" | base64 -d > "output/$name_modifie";
                    rm "deciphered_output.txt";
                    break
                fi
            fi
        done
    fi
done
exit 0;
