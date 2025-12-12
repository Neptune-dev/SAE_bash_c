#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../include/decipherbody.h"
#include "../include/tools.h"

static unsigned char* file;
static char* a;
static char* b;
static char* filename;
static char* encodedKey;

void DecipherGarbageCollector()
{
    ToolsGarbageCollector();

    if (a != NULL)
    {
        printf("Free a\n");
        free(a);
        a = NULL;
    }
    if (b != NULL)
    {
        printf("Free b\n");
        free(b);
        b = NULL;
    }
    if (file != NULL)
    {
        printf("Free file\n");
        free(file);
        file = NULL;
    }
    if (filename != NULL)
    {
        printf("Free filename\n");
        free(filename);
        filename = NULL;
    }
    if (encodedKey != NULL)
    {
        printf("Free encodedKey\n");
        free(encodedKey);
        encodedKey = NULL;
    }
}

int Decipher (char* key, char* target)
{
    size_t fileSize; //taille calculée dans ReadFile
    file = ReadFile(target, &fileSize); // lecture du fichier et taille correcte dans fileSize
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }
    encodedKey = Encode64(key, strlen(key));
    printf("Clef encodee : %s\n", encodedKey);
    //strlen passe pas donc on passe la vraie taille en paramètre de toutes les fonctions d'encodage/décodage et de vignere
    a = Encode64(file, fileSize);
    printf("Encode64 : %s\n", a);
    //FONCTIONNEL !!!!!
    b = Devignere(encodedKey, a);
    printf("Devignere : %s\n", b);
    filename = "deciphered_output.txt";
    WriteFile(filename, b);
    //faire un base64 -d deciphered_output.txt
    //char* c = Decode64(b);
    printf("✓ Dechiffrage termine avec succes !\n");


    return EXIT_SUCCESS;
}