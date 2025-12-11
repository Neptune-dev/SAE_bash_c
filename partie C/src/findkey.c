#include <stdlib.h>
#include <errno.h>

#include "../include/findkeybody.h"

int main (int argc, char* argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc != 3)
    {
        perror("Usage : ./findkey <monfichierclair> <monfichierchiffre>");
        return 1;
    }

    FindKey (argv[1], argv[2]);

    return EXIT_SUCCESS;
}