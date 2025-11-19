#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"

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
    int outputLen = 4 * ((fileLen + 2) / 3); // nombre de caractères base64
    /*
    groupes de 3 octets (3 x 8 = 24 bits) -> 4 caractères Base64 (4 x 6 = 24 bits)
    nombre de groupes de 3o = ⌈ fileLen / 3 ⌉
    outputLen = 4 x ⌈ fileLen / 3 ⌉       car on rappelle 3o = 4 char Base64
              = 4 x ((fileLen + 2) / 3)  car la division entière tronquée par 3, on obtient le plafond en ajoutant 2
    examples :
        si fileLen = 3k   -> (3k + 2) / 3     = k
        si fileLen = 3k+1 -> (3k + 1 + 2) / 3 = k+1
        si fileLen = 3k+2 -> (3k + 2 + 2) / 3 = k+1
    */

    unsigned char* temp = (unsigned char*) malloc (sizeof(unsigned char) * (fileLen + 1)); // fichier de travail, outputLen + 1 pour le '\0
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
    int encodedLen = strlen(s); // taille du fichier à décoder
    int outputLen = encodedLen; // taille de sortie

    char* indexes = (char*) malloc((encodedLen + 1) * sizeof(char));
    /* ficher de travail, qui contient les indexes des caractères de la chaine dans la table de Base64
    ici on utilise des char pour travailler sur 1o plutot que 4o (int) */

    char* output = (char*) malloc((outputLen + 1) * sizeof(char)); // fichier de retour

    for (int i = 0; i < encodedLen; i++)
    {
        indexes[i] = 0;
        while (s[i] != encoding_table[indexes[i]] && indexes[i] <= 63)
        {
            indexes[i]++; // on récupère notre chaine sous forme d'un tableau d'indexes dans la table base64
        }
    }
    indexes[encodedLen] = 0; // on evite les dépassements sur y+1

    for (int i = 0; i < outputLen; i++)
    {

        output[i] = (indexes[0] << 2) | (indexes[1] >> 4);
        /* on prend les 6 bits du premier index, on met deux 0 à droite
        on prend les 2 bits de poids fort de l'index suivant, on les sert à droite
        (un char est sur 8bits mais le caractère Base64 est sur 6bits donc décalage à droite de 4 au lieu de 6)
        on fait les 6bits de poids fort OR les 2bits de poids faible */

        for (int y = 0; y < encodedLen; y++)
        {
            indexes[y] = ((indexes[y + 1] & 15) << 2) | (indexes[y + 2] >> 4);
            /* masque (1111) = 15 pour récuprer les 4 derniers bit
            on met deux 0 à droite
            on ajoute les 2 bit de poids fort de la case suivante
            */
        }
    }

    output[outputLen] = '\0'; //fin de chaine
    free(indexes);
    return output;
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

// dechiffre la chaine table par le chiffre de Vignère avec la clé répétée key, qui doit être en B64
char * Devignere (char* key, char* s)
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

        output[i] = encoding_table[(charIndex - offset + 64) % 64]; //+64 pour éviter les valeurs négatives, le modulo pour boucler dans les tables
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

// fonction pour modifier le fichier d'origine
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
