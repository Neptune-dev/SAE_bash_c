#!/bin/bash


#Si le dossier .sh-toolbox n'existe pas
if [ ! -d ".sh-toolbox" ]; then
    echo "Le dossier .sh-toolbox n'existe pas.";
    exit 1;
fi

#Si le fichier archives n'existe pas
if [ ! -f ".sh-toolbox/archives" ]; then
    echo "Le fichier archives n'existe pas."
    exit 2;
fi

#Tous les fichiers dans archives en enlevant N
fichiers=$(cat .sh-toolbox/archives | tail -n +2);

#si c'est vide: fichier archives vide donc exit 3 pour faciliter la tâche (réinitialisation de archives)
if [ -z "$fichiers" ]; then
    exit 3;
fi
#fichier de la forme nom_fichier:date_creation:contenu_cle
for fichier in $fichiers
do
    estDansArchives=0;
    estDansToolbox=0;
    nom_fichier=$(echo "$fichier" | cut -d":" -f1);

    #cas si une archive dans le fichier archives n'existe pas dans le dossier .sh-toolbox
    for file in $(ls .sh-toolbox/*.gz | cut -d'/' -f2)
    do
        if [[ "$nom_fichier" == "$file" ]]; then
            estDansToolbox=1;
        fi
    done

    if [ $estDansToolbox -eq 0 ]; then
        echo "L'archive $nom_fichier n'existe pas dans le fichier .sh-toolbox.";
        exit 3;
    fi

    #si une archive existe mais n'est pas mentionnée dans le fichier archives
    for filepath in .sh-toolbox/*.gz; do
        base_file=$(basename "$filepath")
        
        # Vérifie si ce fichier est mentionné dans .sh-toolbox/archives
        if ! grep -q "^$base_file:" ".sh-toolbox/archives"; then
            echo "L'archive $base_file existe mais n'est pas metionnée dans .sh-toolbox/archives.";
            exit 3;
        fi
    done

    date_creation=$(echo "$fichier" | cut -d":" -f2);
    contenu_cle=$(echo "$fichier" | cut -d":" -f3);
    if [ -z "$contenu_cle" ]; then
        echo $nom_fichier:$date_creation:$contenu_cle
    fi
done

exit 0;