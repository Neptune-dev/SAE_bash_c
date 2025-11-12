#!/bin/bash

if [ ! -d ".sh-toolbox" ]; then
    echo "Le dossier .sh-toolbox a été créé.";
    mkdir .sh-toolbox;
    exit 0;
fi

if [ ! -d ".sh-toolbox/archives" ]; then
    echo "Le dossier .sh-toolbox/archives a été créé.";
    touch .sh-toolbox/archives;
    echo "0" > .sh-toolbox/archives;
    exit 0;
fi

nb_fichiers=$(ls .sh-toolbox | wc -l);
echo $nb_fichiers;
if [ $nb_fichiers -gt 1 ]; then
    echo "Il y a plus des fichiers existants dans le dossier .sh-toolbox";
    exit 2;
fi

#en cas d'erreur
echo "Le dossier et/ou le fichier n'a pas pu être créé.";
exit 1;