./ls-toolbox.sh

code_erreur=$?

case $code_erreur in
    1) 
        #Si dossier .sh-toolbox manquant, alors le fichier archives aussi donc on peut directement appeler le script init
        echo "Voulez-vous réinitialiser l'environnement de travail en créant le dossier .sh-toolbox ? (oui ou non)";
        read verification;
        if [ $verification == "oui" ]; then
            ./init-toolbox.sh;
        fi
        ;;
    2) 
        #Uniquement l'initisalisation du fichier archives
        echo "Voulez-vous créer le fichier archives ? (oui ou non)";
        read verification;
        if [ $verification == "oui" ]; then
            touch .sh-toolbox/archives;
            echo "0" > .sh-toolbox/archives;
        fi
        ;;
    3)
        #Explication:
        #on veut gérer deux cas: si l'archive dans le fichier archives n'existe pas dans le répertoire .sh-toolbox:
        # -> on remet à jour le fichier archives en remettant N à jour ainsi que toutes les lignes dans archives.
        # cas 2: si l'archive est présente dans .sh-toolbox mais pas mentionnée dans archives:
        # -> remettre à jour le fichier archives gère automatiquement les deux cas, donc aucun soucis
        echo "Voulez-vous réinitialiser le contenu du fichier archives afin de restaurer l'environnement de travail ? (oui ou non)";
        read verification;
        if [ $verification == "oui" ]; then
            N=$(ls .sh-toolbox/*.gz | wc -l); # nombre de lignes dans le répertoire .sh-toolbox -> dossier non corrompu
            echo "$N" > ".sh-toolbox/archives" # > on ajoute directement le nombre de fichiers .gz dans le fichier archives

            for fichier in $(ls .sh-toolbox/*.gz)
            do
                #pour tous les .gz dans le répertoire, on prend le basename et la date d'ajout et on l'ajoute dans le fichier archives
                #(on supprime l'ancien contenu pour afficher le nouveau)
                nom_fichier=$(basename $fichier);
                date_suppr=$(date +"%Y%m%d-%H%M%S");
                echo "$nom_fichier:$date_suppr:" >> ".sh-toolbox/archives"
            done
        fi
    ;;
esac