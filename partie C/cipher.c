#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define KEY_SIZE 150
#define FILENAME_SIZE 150


static char encoding_table[] = {
                                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'
                            };

// encode une chaine en base 64
char * Code64 (char *s)
{

}

// decode une chaine de la base 64
char * Decode64 (char *s)
{

}


// chiffre la chaine table par le chiffre de Vignère avec la clé répétée key
char * Vignere (char* key, char* s)
{
    int size = strlen(s); //taille du fichier
    int keyLen = strlen(key); //taille de la clé
    
    char* output = (char*) malloc (sizeof(char) * size); //fichier de retour

    int charIndex;
    int offset;

    for (int i = 0; i < size; i++)
    {
        // on trouve l'index du caractère de notre chaine
        charIndex = 0;
        while (s[i] != encoding_table[charIndex])
        {
            charIndex++;
        }

        // on trouve l'index de la clef = notre offset
        offset = 0;
        while (key[i % keyLen] != encoding_table[offset])
        {
            offset++;
        }

        output[i] = encoding_table[(charIndex + offset) % 64]; //les modulos pour boucler dans les tables
    }

    return output;
}

// ouvre un fichier et retourne sa chaine de caractère
char * ReadFile (char* fileName)
{
    FILE* file;
    file = fopen(fileName, "r");
    
    int size = 0;
    char c;

    do //on trouve la taille du fichier
    {
        size++;
    } while ((c = fgetc(file)) != EOF);

    // on reserve le bon espace mémoire
    char* output = (char*) malloc (sizeof(char) * size);

    // on revient au début du fichier
    rewind(file);

    // on remplit notre chaine
    for (int i = 0; i <= size - 2; i++)
    {
        output[i] = fgetc(file);
    }
    output[size - 1] = '\0'; // on ajoute le caractère de fin de chaine

    fclose(file);
    return output;
}

void ReplaceFile(char* fileName, char* s)
{
    FILE* file;
    file = fopen(fileName, "w");
    
    for (int i = 0; i < strlen(s); i++)
    {
        fputc(s[i], file);
    }

    fclose(file);
}

int main (int argc, char * argv[])
{
    //on vérifie le nombre d'arguments d'arguments
    if (argc < 3)
    {
        perror("Erreur, les arguments sont invalides");
        return 1;
    }

    char* file = ReadFile(argv[2]);

    char* encryptedFile = Decode64(Vignere(argv[1], Code64(file)));

    ReplaceFile(argv[2], encryptedFile);

    free(file);
    free(encryptedFile);

    return 0;
}