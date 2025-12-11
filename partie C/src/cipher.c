#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tools.h"

int main (int argc, char * argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc != 3)
    {
        perror("Usage : ./cipher <maclef> <monfichierclair>");
        return EXIT_FAILURE;
    }

    size_t fileSize;
    unsigned char* file = ReadFile(argv[2], &fileSize);
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }
    char* a = Encode64(file,fileSize);
    printf("Encode64 : %s\n", a);
    char* b = Vignere(argv[1], a);
    printf("Vignere : %s\n", b);
    free(file);
    char* filename = "ciphered_output.txt";
    WriteFile(filename, b);

    // faire un base64 -d clair.txt > ciphered_output.txt du résultat pour qu'il soit bien ciphered.

    WriteFile(argv[2], b);

    free(b);
    return EXIT_SUCCESS;
}