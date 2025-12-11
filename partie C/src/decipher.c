#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"

int main (int argc, char * argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
    {
        perror("Erreur, les arguments sont invalides");
        return 1;
    }

    char* file = ReadFile(argv[2]);

    char* a = Encode64(file);
    char* b = Devignere(argv[1], a);

    char* encryptedFile = Decode64(b);
    
    printf("f : %s\na : %s\nb : %s\nc : %s\n", file, a, b, encryptedFile);

    free(file);

    free(a);
    free(b);

    ReplaceFile(argv[2], encryptedFile);

    free(encryptedFile);

    return 0;
}