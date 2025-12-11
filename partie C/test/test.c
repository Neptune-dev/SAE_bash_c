#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "tools.h"

char* s1;
char* s2;

void GarbageCollector ()
{
    if (s1 != NULL)
    {
        free(s1);
        s1 = NULL;
    }

    if (s2 != NULL)
    {
        free(s2);
        s2 = NULL;
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
    
    s1 = Encode64(argv[1]);
    if (s1 == NULL)
    {
        perror("Echec d'allocation");
        return EXIT_FAILURE;
    }

    s2 = Decode64(s1);
    if (s2 == NULL)
    {
        perror("Echec d'allocation");
        return EXIT_FAILURE;
    }

    printf("clear   : %s\nencoded : %s\ndecoded : %s\n", argv[1], s1, s2);


    return EXIT_SUCCESS;
}