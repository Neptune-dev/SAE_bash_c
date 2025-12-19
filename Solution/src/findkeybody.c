#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include "include/findkeybody.h"
#include "include/tools.h"

int FindKey (char* def, ...)
{
    va_list argList;
    va_start(argList, def);

    // archive ?
    int taille = strlen(def);
    char* outputFileName = NULL;
    char* enf = NULL;
    int archive = 1; //booléen qui nous permet de savoir si c'est une archive ou pas plus tard
    if (def[taille - 3] != '.' || def[taille - 2] != 'g' || def[taille - 1] != 'z')
    {
        enf = va_arg(argList, char*);
        archive = 0;
    }
    outputFileName = va_arg(argList, char*);
    va_end(argList);

    // récupération des bons fichiers, en fonction de sil s'agit d'une archive ou non
    char* decryptedFile;
    char* encryptedFile;
    size_t fileSize1;
    size_t fileSize2;

    if (archive)
    {
        if (Ungz(def, "fktmp"))
        {
            return EXIT_FAILURE;
        }

        printf("C'est une archive, la v1.0.0 du sujet nous a induit en erreur : cette fonction n'est pas prise en charge.\nMerci de donner directement les fichier (./findkey pour l'aide).\n");

        return EXIT_SUCCESS;
    } else
    {
        decryptedFile = (char*)ReadFile(def, &fileSize1);
        if (!decryptedFile) {
            perror("Erreur lors de la lecture du fichier");
            return EXIT_FAILURE;
        }

        encryptedFile = (char*)ReadFile(enf, &fileSize2);
        if (!encryptedFile) {
            perror("Erreur lors de la lecture du fichier");
            free(decryptedFile);
            return EXIT_FAILURE;
        }
    }
    
    

    // appel de l'outil KeyFinder (le coeur du travail)
    int keySize;
    char* key = KeyFinder(Encode64(decryptedFile, fileSize1), Encode64(encryptedFile, fileSize2), &keySize);

    free(decryptedFile);
    free(encryptedFile);

    // exxport dans un fichier si demandé, sinon sur les sorties standart et d'erreur
    if (outputFileName)
    {
        WriteFile(outputFileName, key);
    } else
    {
        fprintf(stdout, "%s\n", key);
        fprintf(stderr, "%d\n", keySize);
    }

    free(key);

    return EXIT_SUCCESS;
}