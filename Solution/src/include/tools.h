#ifndef TOOLS_H
#define TOOLS_H

char * Encode64 (char *,size_t);
char * Decode64 (char *);
char * Vignere (char *, char *);
char * Devignere (char *, char *);
unsigned char *ReadFile(char *, size_t *);
void WriteFile(char *, char *);
void Ungz (char *, char *);
char * KeyFinder (char *, char *, int *);
int DetectKeyCycle (char *, int);

#endif