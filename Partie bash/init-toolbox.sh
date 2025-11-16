#!/bin/bash
REPERTOIRE=.sh-toolbox;

if [ ! -d "$REPERTOIRE" ]; then
    echo "Le dossier $REPERTOIRE a été créé.";
    mkdir $REPERTOIRE;
    exit 0;
fi

if [ ! -f "$REPERTOIRE/archives" ]; then
    echo "Le dossier $REPERTOIRE/archives a été créé.";
    touch $REPERTOIRE/archives;
    echo "0" > $REPERTOIRE/archives;
    exit 0;
fi



nb_fichiers=$(ls $REPERTOIRE | wc -l);
if [ $nb_fichiers -gt 1 ]; then echo "Des fichiers différents du fichier archives sont présents dans le dossier $REPERTOIRE."; exit 2; fi
if [ $nb_fichiers -eq 1 ]; then echo "0" > $REPERTOIRE/archives; exit 0; fi

#en cas d'erreur
echo "Le dossier et/ou le fichier n'a pas pu être créé.";
exit 1;