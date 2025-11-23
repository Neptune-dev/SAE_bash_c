#!/bin/bash

# Dossier principal
dossier_principal=".sh-toolbox/client1-20250901-1503.tar.gz.tmp/data";

# Date de référence
date_ref=$(grep -i "Accepted password for admin from" .sh-toolbox/client1-20250901-1503.tar.gz.tmp/var/log/auth.log | tail -n 1 | cut -d" " -f1,2,3);
ts_ref=$(date -d "$date_ref" +%s)
echo $date_ref;
# Parcours de tous les sous-dossiers
for sous_dossier in "$dossier_principal"/*/; do
    [ -d "$sous_dossier" ] || continue  # ignore si ce n'est pas un dossier
    # Parcours de tous les fichiers du sous-dossier
    for fichier in "$sous_dossier"*; do
        [ -f "$fichier" ] || continue  # ignore si ce n'est pas un fichier

        # Récupération de la date de modification
        date_modif=$(stat -c "%y" "$fichier" | cut -d'.' -f1)

        # Conversion en timestamp
        ts_file=$(date -d "$date_modif" +%s)

        # Comparaison
        if [ "$ts_file" -gt "$ts_ref" ]; then
            echo $date_modif;
            echo "$fichier → Fichier modifié après la date de connexion";
        fi
    done
done
