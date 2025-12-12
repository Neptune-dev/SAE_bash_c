#ifndef TOOLS_H
#define TOOLS_H

void ToolsGarbageCollector();
char * Encode64 (char *,size_t);
char * Decode64 (char *);
char * Vignere (char *, char *);
char * Devignere (char *, char *);
unsigned char *ReadFile(char* fileName, size_t *outSize);
void WriteFile(char *, char *);
char * KeyFinder (char *, char *, int *);
int DetectKeyCycle (char *, int);

#endif