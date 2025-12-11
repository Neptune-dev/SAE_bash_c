#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"

#define TEMP_PATH "temp"
#define DELETE_TEMP "rm temp"

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


char *Encode64_2(char *s) {
    char command[1024];

    // printf -- permet d’éviter les injections tant que s ne contient pas %
    snprintf(command, sizeof(command), "printf \"%s\" | base64", s);

    FILE *fp = popen(command, "r");
    if (!fp) return NULL;

    char buffer[4096];
    size_t n = fread(buffer, 1, sizeof(buffer), fp);

    pclose(fp);

    // enlever le \n final si présent
    if (n > 0 && buffer[n-1] == '\n') {
        buffer[n-1] = '\0';
        n--;
    }


    char *out = malloc(n + 1);
    memcpy(out, buffer, n);
    out[n] = '\0';

        // Supprimer le padding '='
    while (n > 0 && out[n-1] == '=') {
        out[n-1] = '\0';
        n--;
    }

    return out;
}


char *Encode64(char *s) {
    char command[256];
    snprintf(command, sizeof(command), "base64 -d");

    FILE *fp = popen(command, "w+");
    if (!fp) return NULL;

    fwrite(s, 1, strlen(s), fp);
    fflush(fp);

    // Lire le résultat décodé
    char buffer[20000];
    size_t n = fread(buffer, 1, sizeof(buffer), fp);

    pclose(fp);

    char *out = malloc(n + 1);
    memcpy(out, buffer, n);
    out[n] = '\0';
    return out;
}

// encode une chaine en base 64
char *Decode64_2(char *input, size_t *outLen) {
    if (!input) return NULL;

    size_t len = strlen(input);
    if (len % 4 != 0) {
        // Base64 standard avec padding doit être multiple de 4
        // Ici on suppose padding '=' supprimé, donc on complète avec 0 si nécessaire
        size_t pad = 4 - (len % 4);
        char *tmp = malloc(len + pad + 1);
        strcpy(tmp, input);
        for (size_t i = 0; i < pad; i++) tmp[len + i] = 'A'; // 'A' = 0 en base64
        tmp[len + pad] = '\0';
        input = tmp;
        len += pad;
    }

    // Calcul taille de sortie
    size_t outputLen = len / 4 * 3;
    unsigned char *out = malloc(outputLen);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i += 4) {
        int vals[4];
        for (int k = 0; k < 4; k++) {
            if (input[i + k] >= 'A' && input[i + k] <= 'Z') vals[k] = input[i + k] - 'A';
            else if (input[i + k] >= 'a' && input[i + k] <= 'z') vals[k] = input[i + k] - 'a' + 26;
            else if (input[i + k] >= '0' && input[i + k] <= '9') vals[k] = input[i + k] - '0' + 52;
            else if (input[i + k] == '+') vals[k] = 62;
            else if (input[i + k] == '/') vals[k] = 63;
            else vals[k] = 0; // padding ou caractères invalides remplacés par 0
        }

        out[j++] = (vals[0] << 2) | (vals[1] >> 4);
        if (j < outputLen) out[j++] = (vals[1] << 4) | (vals[2] >> 2);
        if (j < outputLen) out[j++] = (vals[2] << 6) | vals[3];
    }

    if (outLen) *outLen = outputLen;
    return out;
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
// chiffre la chaine s avec Vigenère base64, la clé "key" doit être base64
char *Vignere(char *key, char *s)
{
    if (!key || !s) return NULL;

    int fileLen = strlen(s);
    int keyLen  = strlen(key);

    if (keyLen == 0) return NULL;

    char *output = malloc(fileLen + 1);
    if (!output) return NULL;

    for (int i = 0; i < fileLen; i++)
    {
        // Find char index in Base64 table
        int charIndex = -1;
        for (int j = 0; j < 64; j++)
        {
            if (s[i] == encoding_table[j]) {
                charIndex = j;
                break;
            }
        }

        if (charIndex == -1) {
            free(output);
            return NULL; // caractère non Base64
        }

        // Find key offset
        int offset = -1;
        for (int j = 0; j < 64; j++)
        {
            if (key[i % keyLen] == encoding_table[j]) {
                offset = j;
                break;
            }
        }

        if (offset == -1) {
            free(output);
            return NULL; // clé invalide
        }

        output[i] = encoding_table[(charIndex + offset) % 64];
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

char* ReadFile(char* fileName) {
    FILE* file = fopen(fileName, "rb"); // mode binaire
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0'; // fin de chaîne pour les fonctions C
    fclose(file);
    return buffer;
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

// trouve la clef et met sa taille dans la variable pointée par keySize
char * FindKey (char * decrypted, char * encrypted, int * keySize)
{
    int maxSize = strlen(decrypted); // taille du fichier en clair = taille max de la clef
    char * output = (char*)malloc((maxSize + 1) * sizeof(char)); // fichier de retour

    int deIndex;
    int enIndex;
    int offset;
    char keyChar;
    int tempSize = 0;

    // on récupère la clef pour tous les charactères de la chaine
    for (int i = 0; i < maxSize; i++)
    {
        // on trouve l'index du caractère de notre chaine en clair
        deIndex = 0;
        while (decrypted[i] != encoding_table[deIndex] && deIndex <= 63)
        {
            deIndex++;
        }

        // on trouve l'index du caractère de notre chaine en chiffré
        enIndex = 0;
        while (encrypted[i] != encoding_table[enIndex] && enIndex <= 63)
        {
            enIndex++;
        }

        offset = (enIndex - deIndex + 64) % 64; //+64 pour éviter les valeurs négatives, le modulo pour boucler dans les tables
        keyChar = encoding_table[offset];
        
        output[i] = keyChar;
        tempSize++;
    }

    output[tempSize] = '\0';

    // vérification du key cycling : garder la plus petite période qui reproduit toute la clé
    int cycle = DetectKeyCycle(output, tempSize);
    if (cycle < tempSize)
    {
        *keySize = cycle;
        output[cycle] = '\0';

        char * shrunk = (char*)realloc(output, (cycle + 1) * sizeof(char)); // réallocation pour avoir la bonne taille

        output = shrunk;
    } else
    {
        *keySize = tempSize;
    }

    return output;
}

// retourne la plus petite periode de key, sinon len
int DetectKeyCycle (char * key, int len)
{
    // le plus petit cycle possible
    if (len <= 1)
    {
        return len;
    }

    int ok;
    for (int i = 1; i <= len; i++)
    {
        ok = 1;
        for (int j = 0; j < len; j++)
        {
            if (key[j] != key[j % i])
            {
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            return i;
        }
    }

    return len;
}