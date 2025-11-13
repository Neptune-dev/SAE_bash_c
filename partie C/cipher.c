#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


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
char * Encode64 (char *s)
{
    int fileLen = strlen(s); // taille du fichier
    int outputLen = 4 * ((fileLen + 2) / 3); // nombre de caractères base64 = ceil(fileLen * 8 / 6)

    printf("fileLen : %d | outputLen : %d\n", fileLen, outputLen);

    unsigned char* temp = (unsigned char*) malloc (sizeof(unsigned char) * (fileLen + 1)); // fichier de travail
    char* output = (char*) malloc ((outputLen + 1) * sizeof(char)); // fichier de retour

    for (int i = 0; i < fileLen; i++)
    {
        temp[i] = (unsigned char) s[i]; // on copie tout le tableau
    }
    temp[fileLen] = 0; // on evite les dépassements sur y+1

    for (int i = 0; i < outputLen; i++)
    {
        output[i] = encoding_table[(unsigned char)temp[0] >> 2]; // on prend les 6 premiers bits et les transforme en caractère de la table 64

        for (int y = 0; y < fileLen; y++)
        {
            temp[y] <<= 6; //on pousse à gauche les deux bits de fin de caractère
            temp[y] = temp[y] | ((unsigned char)temp[y + 1] >> 2); // on pousse les 6 premiers bit de la case suivante sur la case courante
        }
    }

    output[outputLen] = '\0'; //fin de chaine
    free(temp);
    return output;
}

// decode une chaine de la base 64
char * Decode64 (char *s)
{

}


// chiffre la chaine table par le chiffre de Vignère avec la clé répétée key, qui doit être en B64
char * Vignere (char* key, char* s)
{
    int fileLen = strlen(s); //taille du fichier
    int keyLen = strlen(key); //taille de la clé
    
    char* output = (char*) malloc (sizeof(char) * (fileLen + 1)); //fichier de retour

    int charIndex;
    int offset;

    for (int i = 0; i < fileLen; i++)
    {
        // on trouve l'index du caractère de notre chaine
        charIndex = 0;
        while (s[i] != encoding_table[charIndex] && charIndex <= 63)
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

    output[fileLen] = '\0';
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

    char* encodedFile = Encode64(file);
    free(file);

    char* encryptedFile = Vignere(argv[1], encodedFile);
    free(encodedFile);

    char* finalFile = Decode64(encryptedFile);
    free(encryptedFile);

    ReplaceFile(argv[2], finalFile);

    free(finalFile);

    return 0;
}