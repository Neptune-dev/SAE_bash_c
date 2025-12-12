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
    char* encryptedKey = Encode64(argv[1], strlen(argv[1]));
    char* a = Encode64(file,fileSize);
    char* b = Vignere(encryptedKey, a);
    printf("Vig %s. \n", b);
    free(file);
    printf("Fichier chiffré avec succès dans le fichier ciphered_output.txt !\nN'oubliez pas d'utiliser base64 -d ciphered_output.txt > monfichierchiffre pour obtenir le fichier chiffré final.\n");
    char* filename = "ciphered_output.txt";
    WriteFile(filename, b);

    // faire un base64 -d clair.txt > ciphered_output.txt du résultat pour qu'il soit bien ciphered.

    WriteFile(argv[2], b);

    free(b);
    free(encryptedKey);
    return EXIT_SUCCESS;
}