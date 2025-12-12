#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../include/decipherbody.h"
#include "../include/tools.h"


int Decipher (char* key, char* target)
{
    size_t fileSize; //taille calculée dans ReadFile
    unsigned char* file = ReadFile(target, &fileSize); // lecture du fichier et taille correcte dans fileSize
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return EXIT_FAILURE;
    }
    char* encodedKey = Encode64(key, strlen(key));
    //strlen passe pas donc on passe la vraie taille en paramètre de toutes les fonctions d'encodage/décodage et de vignere
    char* a = Encode64(file, fileSize);
    char* b = Devignere(encodedKey, a);
    printf("Dechiffrage termine avec succès dans deciphered_output.txt !\nN'oubliez pas d'utiliser base64 -d deciphered_output.txt > monfichierclair pour obtenir le fichier final.\n");
    char* filename = "deciphered_output.txt";
    WriteFile(filename, b);

    free(file);
    free(a);
    free(b);
    free(encodedKey);

    return EXIT_SUCCESS;
}