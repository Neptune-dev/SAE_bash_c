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
    int outputLen = (fileLen * 8 + 5) / 6; // nombre de caractères base64 = ceil(fileLen * 8 / 6)

    printf("fileLen : %d | outputLen : %d\n", fileLen, outputLen);

    char* temp = (char*) malloc (sizeof(char) * fileLen); // fichier de travail
    char* output = (char*) malloc (outputLen / 8); // fichier de retour

    for (int i = 0; i < fileLen; i++)
    {
        temp[i] = s[i]; // on copie tout le tableau
    }
    temp[fileLen] = 0;

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

int main (int argc, char * argv[])
{
    char* a = "Salut";
    printf("Decoded : %s | Encoded : %s\n", a, Encode64(a));
    return 0;
}