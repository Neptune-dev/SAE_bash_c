#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

int main (int argc, char * argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
    {
        perror("Erreur, les arguments sont invalides");
        return 1;
    }

    char* file = ReadFile(argv[2]);

    system("")

    char* encryptedFile = Decode64(Vignere(Encode64(argv[1]), Encode64(file)));
    free(file);

    ReplaceFile(argv[2], encryptedFile);

    free(encryptedFile);

    return 0;
}

/*#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[1024];
    char *command = "ls -la"; // Remplacez par la commande souhaitée

    // Exécuter la commande
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Échec de popen");
        exit(1);
    }

    // Lire la sortie
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    // Fermer le flux
    if (pclose(fp) == -1) {
        perror("Échec de pclose");
        exit(1);
    }

    return 0;
}
*/

/*#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[1024];

    // Vérification des arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <paramètres>\n", argv[0]);
        return 1;
    }

    // Préparer la commande avec des paramètres
    char command[256];
    snprintf(command, sizeof(command), "ls -la %s", argv[1]); // Modifier selon vos besoins

    // Exécuter la commande
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Échec de popen");
        exit(1);
    }

    // Lire la sortie
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    // Fermer le flux
    if (pclose(fp) == -1) {
        perror("Échec de pclose");
        exit(1);
    }

    return 0;
}
*/