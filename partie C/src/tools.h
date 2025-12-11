#ifndef TOOLS_H
#define TOOLS_H


char * Encode64_2(char *);
char * Decode64_2(char *,size_t* );
char * Encode64 (char *);
char * Decode64 (char *);
char * Vignere (char *, char *);
char * Devignere (char *, char *);
char * ReadFile (char *);
void ReplaceFile(char *, char *);
char * FindKey (char *, char *, int *);
int DetectKeyCycle (char *, int);
#endif