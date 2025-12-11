#include <errno.h>
#include <stdlib.h>
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

    char* file = ReadFile(argv[2]);

    char* a = Encode64_2(argv[1]);
    printf("clé encodée : %s\n", a);
    char* b = Encode64_2(file);
    printf("fichier encodé : %s\n", b);
    char* c = Vignere(a,b);
    printf("vigenere : %s\n", c);

    size_t* outlen;
    char* encryptedFile = Devignere(a,c);
    printf("Dévigenere: %s\n",encryptedFile);
    char* final = Decode64_2(encryptedFile,outlen);
    printf("Final: %s\n",final);
    free(file);

    free(a);
    free(b);
    free(c);

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