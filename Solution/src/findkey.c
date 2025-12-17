#include <stdlib.h>
#include <errno.h>

#include "include/findkeybody.h"

int BadUsage()
{
    perror("Usage : ./findkey <monfichierclair> <monfichierchiffre> [options]\nOptions:\n\t-o <fichierdesortie>\tStocke la clef dans un fichier de sortie");
    return EXIT_FAILURE;
}

int main (int argc, char* argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
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

    //on vérifie que FindKey renvoie 0 parce que sinon c'est que l'ouverture de fichier a échoué
    if (FindKey (argv[1], argv[2], output) != 0)
    {
        perror("FindKey à échoué");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}