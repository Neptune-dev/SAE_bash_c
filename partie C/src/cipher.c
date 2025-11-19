#include <errno.h>
#include <stdlib.h>
#include <string.h>

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
    char* encryptedFile = Decode64(Vignere(argv[1], Encode64(file)));
    free(file);

    ReplaceFile(argv[2], encryptedFile);

    free(encryptedFile);

    return 0;
}