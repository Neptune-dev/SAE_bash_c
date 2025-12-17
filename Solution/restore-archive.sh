#!/bin/bash

REPERTOIRE=".sh-toolbox"

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <destination-directory>"
    exit 1
fi

DEST_DIR="$1"

#code de retour 1 si erreur sur .sh-toolbox ou fichier archives manquant
if [ ! -d "$REPERTOIRE" ]; then
    echo "Le répertoire $REPERTOIRE n'existe pas."
    exit 1
fi

if [ ! -f "$REPERTOIRE/archives" ]; then
    echo "Le fichier $REPERTOIRE/archives n'existe pas."
    exit 1
fi

#on vérifie qu'il y a au moins une archive .gz dans le répertoire sinon on créé pas le dossier de destination
if ! ls "$REPERTOIRE"/*.gz >/dev/null 2>&1; then
    echo "Aucune archive trouvée."
    exit 1
fi

# dossier de destination à créer s'il n'existe pas, code de retour 2 si échec de création
if [ ! -d "$DEST_DIR" ]; then
    mkdir -p "$DEST_DIR"
    if [ $? -ne 0 ]; then
        echo "Erreur: Impossible de créer le dossier de destination '$DEST_DIR'."
        exit 2
    fi
fi

#on liste les archives disponibles et l'utilisateur en choisit une
echo "Liste des archives disponibles :"
N=1
TOTAL=0

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
tar -xzf "$REPERTOIRE/$fichier" -C "$output_dir";
# si échec de la commande tar
if [ $? -ne 0 ]; then echo "La décompression du fichier a échouée."; exit 3; fi

#s'il n y a pas de fichier logs
if [ ! -f "$output_dir/var/log/auth.log" ]; then exit 4; fi
#s'il n'y a pas de dossiers dans /data
if [ $(ls "$output_dir/data"| wc -w) -eq 0 ]; then exit 5; fi


# Date de référence (date de dernière connexion sur le compte admin) + conversion en timestamp pour comparaison
date_ref=$(grep -i "Accepted password for admin from" "$output_dir/var/log/auth.log" | tail -n 1 | cut -d" " -f1,2,3);
ts_ref=$(date -d "$date_ref" +%s);


grep -rl "" "$DEST_DIR/data" | while read -r chiffre; do
    [ -f "$chiffre" ] || continue

    mtime_chiffre=$(stat -c %Y "$chiffre" 2>/dev/null) || continue
    [ "$mtime_chiffre" -le "$ts_ref" ] && continue
    base_suspect=$(basename "$chiffre")
    # -------- 3. Recherche du clair correspondant --------
    grep -rl "" "$DEST_DIR" | while read -r clair; do
        [ -f "$clair" ] || continue

        mtime_clair=$(stat -c %Y "$clair" 2>/dev/null) || continue
        [ "$mtime_clair" -ge "$ts_ref" ] && continue

        if [ "$(basename "$clair")" = "$base_suspect" ]; then
            # -------- 4. Appel de findkey + base64 -d pour déchiffrer --------
            key_b64=$(./findkey "$clair" "$chiffre")

            if [ -z "$key_b64" ]; then
                echo "[ERREUR] findkey n'a retourné aucune clé"
                break
            fi

            key=$(echo "$key_b64" | base64 -d 2>/dev/null);
            echo "[CLE DECHIFFREE] $key"
            break
        fi
    done
done
