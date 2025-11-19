#ifndef TOOLS_H
#define TOOLS_H

char * Encode64 (char *);
char * Decode64 (char *);
char * Vignere (char *, char *);
char * Devignere (char *, char *);
char * ReadFile (char *);
void ReplaceFile(char *, char *);

#endif