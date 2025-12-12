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

    // lecture du fichier via ReadFile et taille correcte dans fileSize (passage par adresse)
    size_t fileSize;
    unsigned char* file = ReadFile(argv[2], &fileSize);
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }
    char* encryptedKey = Encode64(argv[1], strlen(argv[1]));
    char* a = Encode64(file,fileSize);
    char* b = Vignere(encryptedKey, a);
    printf("Fichier chiffré avec succès dans le fichier ciphered_output.txt !\nN'oubliez pas d'utiliser base64 -d ciphered_output.txt > monfichierchiffre pour obtenir le fichier chiffré final.\n");
    char* filename = "ciphered_output.txt";
    WriteFile(filename, b);

    free(a);
    free(b);
    free(file);
    free(encryptedKey);
    return EXIT_SUCCESS;
}