#!/bin/bash
REPERTOIRE=.sh-toolbox;

if [ ! -d "$REPERTOIRE" ]; then echo "Le fichier $REPERTOIRE n'existe pas."; exit 1; fi

forcer=0;
#parcourt tous les fichiers passés en paramètre
for fichier in "$@"
do
    #bonus: option -f avec deux paramètres = erreur
    if [ $fichier == "-f" ]; then
        if [ $# -ne 2 ]; then echo "Un seul paramètre avec l'option -f"; exit 5; fi
        
        forcer=1;
        continue;
    fi
    chemin=$fichier;
    nom_fichier=$(basename $chemin);

    #nb au début du fichier
    N=$(head -1 $REPERTOIRE/archives);

    #si fichier inexistant
    if [ ! -f "$chemin" ]; then echo "Le chemin $chemin passé en paramètre n'existe pas."; exit 2; fi

    #si le fichier n'est pas déjà copié dans archives, on le copie
    if [ ! -f "$REPERTOIRE/$nom_fichier" ]; then
        cp $chemin $REPERTOIRE/;
        #cp peut éventuellement générer une erreur donc on gère
        if [ $? -ne 0 ]; then echo "Un soucis a eu lieu lors de la copie du fichier dans $REPERTOIRE."; exit 3; fi

        #ajout d'un fichier -> incrémentation de N
        N=$(($N+1));
        #copie du fichier initial sauf le chiffre N au début dans un fichier tmp
        tail -n +2 "$REPERTOIRE/archives" > "$nom_fichier.tmp"
        #fait rentrer le nouveau N au tout début du fichier (fichier vide initialement donc > suffit)
        echo "$N" > "$REPERTOIRE/archives"
        # >> c'est le contenu du premier fichier + le contenu du 2eme fichier (fichier non vide donc concaténation obligatoire >>)
        cat "$nom_fichier.tmp" >> "$REPERTOIRE/archives"
        rm "$nom_fichier.tmp"

        #date d'ajout du fichier dans l'archive = date correspondante à l'exécution du code
        date_ajout=$(date +"%Y%m%d-%H%M%S");
        #ajout de la nouvelle ligne dans le fichier archives (nouvelle ligne = nouvelle archive)
        echo "$nom_fichier:$date_ajout:" >> "$REPERTOIRE/archives"
    else
        #passage en force donc on copie quitte à écraser un fichier avec un nom identique
        if [ $forcer -eq 1 ]; then
            cp "$chemin" "$REPERTOIRE/"
            if [ $? -ne 0 ]; then echo "Un soucis a eu lieu lors de la copie du fichier dans $REPERTOIRE."; exit 3; fi

            #copie du fichier initial sauf le chiffre N au début dans un fichier tmp
            tail -n +2 "$REPERTOIRE/archives" > "$nom_fichier.tmp"
            #fait rentrer le nouveau N au tout début du fichier (fichier vide initialement donc > suffit)
            echo "$N" > "$REPERTOIRE/archives"
            # >> c'est le contenu du premier fichier + le contenu du 2eme fichier (fichier non vide donc concaténation obligatoire >>)
            cat "$nom_fichier.tmp" >> "$REPERTOIRE/archives"
            rm "$nom_fichier.tmp"

            #date d'ajout du fichier dans l'archive = date correspondante à l'exécution du code
            date_ajout=$(date +"%Y%m%d-%H%M%S");
            #écrasement d'un fichier -> on recherche ce fichier dans le dossier et on l'écrase
            { grep -v "$nom_fichier" $REPERTOIRE/archives; echo "$nom_fichier:$date_ajout:" ; } > "$REPERTOIRE/archives.tmp";
            cp $REPERTOIRE/archives.tmp $REPERTOIRE/archives && rm $REPERTOIRE/archives.tmp;

        else
            echo "Le fichier $nom_fichier existe déjà dans l'archive, voulez-vous l'écraser ? (oui ou non)";
            read verification;

            if [ "$verification" == "oui" ]; then
                cp "$chemin" $REPERTOIRE/;
                if [ $? -ne 0 ]; then echo "Un soucis a eu lieu lors de la copie du fichier dans $REPERTOIRE."; exit 3; fi

                #copie du fichier initial sauf le chiffre N au début dans un fichier tmp
                tail -n +2 "$REPERTOIRE/archives" > "$nom_fichier.tmp"
                #fait rentrer le nouveau N au tout début du fichier (fichier vide initialement donc > suffit)
                echo "$N" > "$REPERTOIRE/archives"
                # >> c'est le contenu du premier fichier + le contenu du 2eme fichier (fichier non vide donc concaténation obligatoire >>)
                cat "$nom_fichier.tmp" >> "$REPERTOIRE/archives"
                rm "$nom_fichier.tmp"
                #date d'ajout du fichier dans l'archive = date correspondante à l'exécution du code
                date_ajout=$(date +"%Y%m%d-%H%M%S");
                #écrasement d'un fichier -> on recherche ce fichier dans le dossier et on l'écrase
                { grep -v "$nom_fichier" $REPERTOIRE/archives; echo "$nom_fichier:$date_ajout:" ; } > "$REPERTOIRE/archives.tmp";
                cp $REPERTOIRE/archives.tmp $REPERTOIRE/archives && rm $REPERTOIRE/archives.tmp;

            elif [ "$verification" == "non" ]; then
                echo "Copie annulée";
            fi
        fi
    fi
done

exit 0;