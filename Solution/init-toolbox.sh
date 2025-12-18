#!/bin/bash
REPERTOIRE=.sh-toolbox;

#création du répertoire s'il existe pas
if [ ! -d "$REPERTOIRE" ]; then
    echo "Le dossier $REPERTOIRE a été créé.";
    mkdir $REPERTOIRE;
    exit 0;
fi

#création du fichier archives s'il existe pas + init à 0
if [ ! -f "$REPERTOIRE/archives" ]; then
    echo "Le dossier $REPERTOIRE/archives a été créé.";
    touch $REPERTOIRE/archives;
    echo "0" > $REPERTOIRE/archives;
    exit 0;
fi

if [[ ! -f "cipher" && ! -f "decipher" ]]; then
    if [[ ! -f "src/decipher.c" || ! -f "src/include/decipherbody.c" || ! -f "src/decipherbody.c" || ! -f "src/makefile" || ! -f "src/tools.c" || ! -f "src/include/tools.h" ||! -f "src/cipher.c" ]]; then
        echo "Les fichiers sources nécessaires à la compilation des binaires sont manquants.";
        exit 10;
    fi
    cd src;
    #gérer si le compilateur n'est pas disponible -> erreur
    if ! command -v gcc &> /dev/null; then
        echo "Erreur: gcc n'est pas installé ou n'est pas dans le PATH.";
        exit 11;
    fi
    #si make échoue on gère l'erreur
    make;
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation des fichiers sources.";
        exit 12;
    fi
    cd ..;
fi


#nombre de fichiers dans le repertoire
nb_fichiers=$(ls $REPERTOIRE | wc -l);
#si y'en a qu'un seul (archives), alors archives vaut 0.
if [ $nb_fichiers -eq 1 ]; then echo "0" > $REPERTOIRE/archives; exit 0; fi
#sinon retour code d'erreur correspondant
if [ $nb_fichiers -gt 1 ]; then echo "Des fichiers différents du fichier archives sont présents dans le dossier $REPERTOIRE."; exit 2; fi

#en cas d'erreur
echo "Le dossier et/ou le fichier n'a pas pu être créé.";
exit 1;