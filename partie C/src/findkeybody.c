#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../include/findkeybody.h"
#include "../include/tools.h"

static char* decryptedFile;
static char* encryptedFile;
static char* key;

void FindkeyGarbageCollector ()
{
    ToolsGarbageCollector();

    if (decryptedFile != NULL)
    {
        free(decryptedFile);
        decryptedFile = NULL;
    }
    if (encryptedFile != NULL)
    {
        free(encryptedFile);
        encryptedFile = NULL;
    }
    if (key != NULL)
    {
        free(key);
        key = NULL;
    }
}

int FindKey (char* def, char* enf)
{
    size_t fileSize1;
    decryptedFile = ReadFile(def, &fileSize1);
    if (!decryptedFile) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }

    size_t fileSize2;
    encryptedFile = ReadFile(enf, &fileSize2);
    if (!encryptedFile) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }

    int keySize;
    key = KeyFinder(Encode64(decryptedFile, fileSize1), Encode64(encryptedFile, fileSize2), &keySize);


    fprintf(stdout, "%s\n", key);
    fprintf(stderr, "%d\n", keySize);


    return EXIT_SUCCESS;
}