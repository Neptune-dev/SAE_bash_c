#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "tools.h"

char* s;

void GarbageCollector ()
{
    if (s != NULL)
    {
        free(s);
        s = NULL;
    }
}

int main (int argc, char** argv)
{
    if (atexit(GarbageCollector) != 0)
    {
        perror("Erreur d'enregistrement du garbage collector");
        return EXIT_FAILURE;
    }
    
    if (argc != 2)
    {
        perror("Mauvais nombre d'arguments");
        return EXIT_FAILURE;
    }    
    
    s = Encode64(argv[1]);

    if (s == NULL)
    {
        perror("Echec d'allocation");
        return EXIT_FAILURE;
    }

    printf("in  : %s\nout : %s\n", argv[1], s);


    return EXIT_SUCCESS;
}