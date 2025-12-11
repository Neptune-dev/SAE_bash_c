#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    size_t fileSize;
    unsigned char* file = ReadFile(argv[2], &fileSize);
    if (!file) {
        perror("Erreur lors de la lecture du fichier");
        return 1;
    }
    char* a = Encode64(file,fileSize);
    printf("Encode64 : %s\n", a);
    char* b = Vignere(argv[1], a);
    printf("Vignere : %s\n", b);
    free(file);
    char* filename = "ciphered_output.txt";
    writeToFile(filename, b);

    // faire un base64 -d clair.txt > ciphered_output.txt du résultat pour qu'il soit bien ciphered.

    ReplaceFile(argv[2], b);

    free(b);
    return 0;
}