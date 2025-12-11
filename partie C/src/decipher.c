#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"

int main (int argc, char * argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
    {
        perror("Erreur, les arguments sont invalides");
        return 1;
    }

    size_t fileSize; //taille calculée dans ReadFile
    unsigned char* file = ReadFile(argv[2], &fileSize); // lecture du fichier et taille correcte dans fileSize
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return 1;
    }
    //strlen passe pas donc on passe la vraie taille en paramètre de toutes les fonctions d'encodage/décodage et de vignere
    char* a = Encode64(file, fileSize);
    printf("Encode64 : %s\n", a);
    //FONCTIONNEL !!!!!
    char* b = Devignere(argv[1], a);
    printf("Devignere : %s\n", b);
    //Pas encore...
    char* encryptedFile = Decode64(b, strlen(b));
    printf("Decode64 : %s\n", encryptedFile);

    ReplaceFile(argv[2], encryptedFile);
    printf("✓ Déchiffrage terminé avec succès !\n");

    free(file);
    free(a);
    free(b);
    free(encryptedFile);

    return 0;
}