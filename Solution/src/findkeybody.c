#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "include/findkeybody.h"
#include "include/tools.h"

int FindKey (char* def, char* enf)
{
    size_t fileSize1;
    char* decryptedFile = ReadFile(def, &fileSize1);
    if (!decryptedFile) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }

    size_t fileSize2;
    char* encryptedFile = ReadFile(enf, &fileSize2);
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