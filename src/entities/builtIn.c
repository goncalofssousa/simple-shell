#include "entities/builtIn.h"
#include <stdlib.h>
#include <string.h>

BuiltIn *createBuiltIn(char *name, int (*execution)(char **args)){
    BuiltIn *new = malloc(sizeof(BuiltIn));

    new->name = malloc(strlen(name) + 1);
    strcpy(new->name, name); 
    new->execution = execution;
    
    return new; 
}

void freeBuiltIn(BuiltIn *builtIn){
    free(builtIn->name); 
    free(builtIn); 
}