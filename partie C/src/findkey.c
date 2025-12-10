#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "tools.h"

int main (int argc, char* argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
    {
        perror("Erreur, les arguments sont invalides");
        return 1;
    }

    char* decryptedFile = ReadFile(argv[1]);
    char* encryptedFile = ReadFile(argv[2]);

    int keySize;
    char* key = FindKey(Encode64(decryptedFile), Encode64(encryptedFile), &keySize);

    free(decryptedFile);
    free(encryptedFile);

    fprintf(stdout, "%s\n", key);
    fprintf(stderr, "%d\n", keySize);

    free(key);

    return 0;
}