#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"


void writeToFile(const char* filename, const char* content) {
    // Ouvrir le fichier en mode écriture ("w" pour écriture texte)
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    // Écrire le contenu dans le fichier
    fprintf(file, "%s", content);

    // Fermer le fichier
    fclose(file);
    printf("Contenu écrit avec succès dans %s\n", filename);
}

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
    char* filename = "deciphered_output.txt";
    writeToFile(filename, b);
    //char* c = Decode64(b);
    printf("✓ Déchiffrage terminé avec succès !\n");

    free(file);
    free(a);
    free(b);

    return 0;
}

