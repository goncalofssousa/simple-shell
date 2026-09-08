#ifndef BUILTIN_H
#define BUILTIN_H

typedef struct builIn {
    char *name; 
    int (*execution)(char **args); 
} BuiltIn; 

BuiltIn *createBuiltIn(char *name, int (*execution)(char **args));
void freeBuiltIn(BuiltIn *builtIn);
#endif