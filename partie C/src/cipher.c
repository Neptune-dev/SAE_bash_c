#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tools.h"

char* a;
char* b;
unsigned char* file;
char* filename;
char* encryptedKey;

void CipherGarbageCollector ()
{
    ToolsGarbageCollector();
    
    if (a != NULL)
    {
        free(a);
        a = NULL;
    }
    if (b != NULL)
    {
        free(b);
        b = NULL;
    }
    if (file != NULL)
    {
        free(file);
        file = NULL;
    }
    if (filename != NULL)
    {
        free(filename);
        filename = NULL;
    }
    if (encryptedKey != NULL)
    {
        free(encryptedKey);
        encryptedKey = NULL;
    }
}

int main (int argc, char * argv[])
{
    atexit(CipherGarbageCollector);

    //on vérifie le nombre d'arguments d'arguments
    if (argc != 3)
    {
        perror("Usage : ./cipher <maclef> <monfichierclair>");
        return EXIT_FAILURE;
    }

    size_t fileSize;
    file = ReadFile(argv[2], &fileSize);
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }
    encryptedKey = Encode64(argv[1], strlen(argv[1]));
    printf("Clef Encodée : %s\n", encryptedKey);
    a = Encode64(file,fileSize);
    printf("Encode64 : %s\n", a);
    b = Vignere(argv[1], a);
    printf("Vignere : %s\n", b);
    filename = "ciphered_output.txt";
    WriteFile(filename, b);

    // faire un base64 -d clair.txt > ciphered_output.txt du résultat pour qu'il soit bien ciphered.

    WriteFile(argv[2], b);


    return EXIT_SUCCESS;
}