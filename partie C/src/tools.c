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
char * Encode64 (char *s,size_t size)
{
    int outputLen = 4 * ((size + 2) / 3); // nombre de caractères base64
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

    // on vérifie si on doit ajouter des '=' de padding auquel cas il faudra tronquer la sortie
    int padding = 0;
    if (size % 3 != 0)
    {
        padding = 3 - (size % 3); // nombre de '=' à supposéent ajouter
        outputLen -= padding; // on tronque la sortie
    }

    unsigned char* temp = (unsigned char*) malloc (sizeof(unsigned char) * (size + 1)); // fichier de travail, outputLen + 1 pour le '\0
    char* output = (char*) malloc ((outputLen + 1) * sizeof(char)); // fichier de retour

    for (int i = 0; i < size; i++)
    {
        temp[i] = (unsigned char) s[i]; // on copie tout le tableau
    }
    temp[size] = 0; // on evite les dépassements sur y+1
    for (int i = 0; i < outputLen; i++)
    {
        output[i] = encoding_table[(unsigned char)temp[0] >> 2]; // on prend les 6 premiers bits et les transforme en caractère de la table 64

        for (int y = 0; y < size; y++)
        {
            temp[y] <<= 6; //on pousse à gauche les deux bits de fin de caractère
            temp[y] = temp[y] | ((unsigned char)temp[y + 1] >> 2); // on pousse les 6 premiers bit de la case suivante sur la case courante
        }
    }

    output[outputLen] = '\0'; //fin de chaine
    free(temp);
    return output;
}

// Fonction pour récupérer l'index d'un caractère Base64 dans la table
int get_base64_index(char c) {
    for (int i = 0; i < 64; i++) {
        if (encoding_table[i] == c) {
            return i;
        }
    }
    return -1; // Retourne -1 si le caractère n'est pas trouvé
}

// Fonction de décodage Base64
char* Decode64(char* s,size_t size) {
    if (!s) return NULL;

    char command[4096];
    snprintf(command, sizeof(command), "base64 -d %s 2>/dev/null", s);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    size_t cap = 8192;
    size_t len = 0;
    char *out = malloc(cap);
    if (!out) {
        pclose(fp);
        return NULL;
    }

    const size_t CHUNK = 4096;
    char buf[CHUNK];
    size_t n;
    while ((n = fread(buf, 1, CHUNK, fp)) > 0) {
        if (len + n + 1 > cap) {
            while (len + n + 1 > cap) cap *= 2;
            char *tmp = realloc(out, cap);
            if (!tmp) {
                free(out);
                pclose(fp);
                return NULL;
            }
            out = tmp;
        }
        memcpy(out + len, buf, n);
        len += n;
    }

    pclose(fp);

    char *shrunk = realloc(out, len + 1);
    if (shrunk) out = shrunk;
    out[len] = '\0';
    return out;
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
unsigned char* ReadFile(char* fileName, size_t* outSize) {
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        *outSize = 0;
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size < 0) {
        perror("Erreur détermination taille");
        fclose(file);
        *outSize = 0;
        return NULL;
    }
    
    unsigned char* output = malloc(size);  // Ne pas ajouter +1, pas besoin du '\0'
    if (!output) {
        perror("Erreur allocation mémoire");
        fclose(file);
        *outSize = 0;
        return NULL;
    }
    
    // Lire caractère par caractère
    int c;
    size_t i = 0;
    while ((c = fgetc(file)) != EOF && i < (size_t)size) {
        output[i] = (unsigned char)c;  // Stocker le caractère comme un octet
        i++;
    }
    
    if (i != (size_t)size) {
        printf("Erreur: %zu bytes lus au lieu de %ld\n", i, size);
    }
    
    fclose(file);
    
    *outSize = (size_t)size;
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