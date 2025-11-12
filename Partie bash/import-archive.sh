#!/bin/bash

if [ ! -d ".sh-toolbox" ]; then
    echo "Le fichier .sh-toolbox n'existe pas.";
    exit 1;
fi

forcer=0;
#parcourt tous les fichiers passés en paramètre
for fichier in "$@"
do
    #bonus: option -f avec deux paramètres = erreur
    if [ $fichier == "-f" ]; then
        if [ $# -ne 2 ]; then
            echo "Un seul paramètre avec l'option -f";
            exit 5;
        fi
        forcer=1;
        continue;
    fi
    chemin=$fichier;
    nom_fichier=$(basename $chemin);
    #nb au début du fichier
    N=$(head -1 .sh-toolbox/archives);
    #si fichier inexistant
    if [ ! -f "$chemin" ]; then
        echo "Le chemin passé en paramètre n'existe pas.";
        exit 2;
    fi
    #si le fichier n'est pas déjà copié dans archives, on le copie
    if [ ! -f ".sh-toolbox/$nom_fichier" ]; then
        cp $chemin .sh-toolbox/;
        #cp peut éventuellement générer une erreur donc on gère
        if [ $? -ne 0 ]; then
            echo "Un soucis a eu lieu lors de la mise à jour du fichier archives.";
            exit 4;
        fi
        #ajout d'un fichier -> incrémentation de N
        N=$(($N+1));
        #on affiche tous les fichiers sauf le chiffre au début et on le fout dans un fichier tmp
        tail -n +2 ".sh-toolbox/archives" > "$nom_fichier.tmp"
        #affiche le nouveau N au tout début du fichier
        echo "$N" > ".sh-toolbox/archives"
        # >> c'est le contenu du premier fichier + le contenu du 2eme fichier
        cat "$nom_fichier.tmp" >> ".sh-toolbox/archives"
        rm "$nom_fichier.tmp"

        date_suppr=$(date +"%Y%m%d-%H%M%S");
        echo "$nom_fichier:$date_suppr:" >> ".sh-toolbox/archives"
    else
        if [ $forcer -eq 1 ]; then
            cp "$chemin" ".sh-toolbox/"
        else
            echo "Le fichier $nom_fichier existe déjà dans l'archive, voulez-vous l'écraser ? (oui ou non)";
            read verification;

            if [ "$verification" == "oui" ]; then
                cp "$chemin" .sh-toolbox/;
                if [ $? -ne 0 ]; then
                    echo "Un soucis a eu lieu lors de la copie du fichier dans .sh-toolbox.";
                    exit 3;
                fi

            elif [ "$verification" == "non" ]; then
                echo "Copie annulée";
            fi
        fi
    fi
done

exit 0;