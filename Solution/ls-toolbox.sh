#!/bin/bash
REPERTOIRE=.sh-toolbox;

#Si le dossier .sh-toolbox n'existe pas
if [ ! -d "$REPERTOIRE" ]; then echo "Le dossier $REPERTOIRE n'existe pas."; exit 1; fi

#Si le fichier archives n'existe pas
if [ ! -f "$REPERTOIRE/archives" ]; then echo "Le fichier archives n'existe pas." exit 2; fi

#Tous les fichiers dans archives en enlevant N
fichiers=$(cat $REPERTOIRE/archives | tail -n +2);

#si c'est vide: fichier archives vide donc exit 3 pour faciliter la tâche (réinitialisation de archives)
if [ -z "$fichiers" ]; then exit 3; fi

#fichier de la forme nom_fichier:date_creation:contenu_cle
for fichier in $fichiers
do
    estDansToolbox=0;
    nom_fichier=$(echo "$fichier" | cut -d":" -f1);

    fichiers_gz=$(ls "$REPERTOIRE/"*.gz | cut -d'/' -f2)
    if [ $? -ne 0 ]; then echo "Aucun fichier .gz dans le répertoire $REPERTOIRE."; exit 0;fi
    #cas si une archive dans le fichier archives n'existe pas dans le dossier .sh-toolbox
    for file in $fichiers_gz
    do
        if [[ "$nom_fichier" == "$file" ]]; then
            estDansToolbox=1;
        fi
    done

    if [ $estDansToolbox -eq 0 ]; then echo "L'archive $nom_fichier n'existe pas dans le fichier $REPERTOIRE."; exit 3; fi

    #si une archive existe mais n'est pas mentionnée dans le fichier archives
    for filepath in $REPERTOIRE/*.gz; do
        base_file=$(basename "$filepath")
        
        # Vérifie si ce fichier est mentionné dans .sh-toolbox/archives
        if ! grep -q "^$base_file:" "$REPERTOIRE/archives"; then
            echo "L'archive $base_file existe mais n'est pas metionnée dans $REPERTOIRE/archives.";
            exit 3;
        fi
    done

    date_creation=$(echo "$fichier" | cut -d":" -f2);
    contenu_cle=$(echo "$fichier" | cut -d":" -f3);
    echo $nom_fichier:$date_creation:$contenu_cle
done

exit 0;