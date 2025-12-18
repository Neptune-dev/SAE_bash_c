#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "include/findkeybody.h"

int BadUsage()
{  
    perror("\nUsage : findkey <fichiers> [options]\n\nFichiers possibles:\n\t<archive>\t\t\t\tArchive.tar.gz\n\t<fichierclair> <fichierchiffre>\t\tNoms de deux fichiers a traiter\n\nOptions:\n\t-o <fichierdesortie>\tStocke la clef dans un fichier de sortie");
    return EXIT_FAILURE;
}

int main (int argc, char* argv[])
{
    //on vérifie le nombre d'arguments minimal (2)
    if (argc == 1)
    {
        return BadUsage();
    }

    int archive = 0; //booléen qui nous permet de savoir si c'est une archive ou pas
    int taille = strlen(argv[1]);

    //on a une archive, sinon deux noms de fichiers
    if (argv[1][taille - 3] == '.' | argv[1][taille - 2] == 'g' | argv[1][taille - 1] == 'z')
    {
        archive = 1;
    } else if (argc < 3) //on vérifie le nombre d'arguments minimal dans le cas des noms de fichiers
    {
        return BadUsage();
    }

    //on vérifie les options
    char* output = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == 'o')
        {
            if ((argc - i)  != 2)
            {
                return BadUsage();
            } else
            {
                output = argv[i + 1];
                break;
            }
        }
    }

    // on peut enfin executer la fonction FindKey
    if (archive)
    {
        if (FindKey (argv[1], output) != 0) //on vérifie que FindKey renvoie 0 parce que sinon c'est que l'ouverture de fichier a échoué
        {
            perror("FindKey a echoue");
            return EXIT_FAILURE;
        }
    } else if (FindKey (argv[1], argv[2], output) != 0) //on vérifie que FindKey renvoie 0 parce que sinon c'est que l'ouverture de fichier a échoué
    {
        perror("FindKey a echoue");
        return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}