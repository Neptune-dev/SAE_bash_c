#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "tools.h"

int main (int argc, char* argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc != 3)
    {
        perror("Usage : ./findkey <monfichierclair> <monfichierchiffre>");
        return 1;
    }

    size_t fileSize1;
    char* decryptedFile = ReadFile(argv[2], &fileSize1);
    if (!decryptedFile) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }

    size_t fileSize2;
    char* encryptedFile = ReadFile(argv[2], &fileSize2);
    if (!encryptedFile) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }

    int keySize;
    char* key = KeyFinder(Encode64(decryptedFile, fileSize1), Encode64(encryptedFile, fileSize2), &keySize);

    free(decryptedFile);
    free(encryptedFile);

    fprintf(stdout, "%s\n", key);
    fprintf(stderr, "%d\n", keySize);

    free(key);

    return EXIT_SUCCESS;
}