#include <stdlib.h>
#include <errno.h>

#include "../include/decipherbody.h"

int main (int argc, char * argv[])
{
    atexit(DecipherGarbageCollector);
    
    //on vérifie le nombre d'arguments d'arguments
    if (argc != 3)
    {
        perror("Usage : ./decipher <maclef> <monfichierchiffre>");
        return EXIT_FAILURE;
    }

    Decipher(argv[1], argv[2]);

    return EXIT_SUCCESS;
}

