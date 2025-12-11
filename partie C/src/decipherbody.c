#include <stdlib.h>
#include <stdio.h>
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

    //strlen passe pas donc on passe la vraie taille en paramètre de toutes les fonctions d'encodage/décodage et de vignere
    char* a = Encode64(file, fileSize);
    printf("Encode64 : %s\n", a);
    //FONCTIONNEL !!!!!
    char* b = Devignere(key, a);
    printf("Devignere : %s\n", b);
    char* filename = "deciphered_output.txt";
    WriteFile(filename, b);
    //faire un base64 -d deciphered_output.txt
    //char* c = Decode64(b);
    printf("✓ Déchiffrage terminé avec succès !\n");

    free(file);
    free(a);
    free(b);

    return EXIT_SUCCESS;
}