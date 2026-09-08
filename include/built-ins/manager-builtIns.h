#ifndef MANAGER_BUILTINS_H
#define MANAGER_BUILTINS_H

#include <glib.h>

typedef struct manager_builtIns {
    GHashTable *builtIns; 
} ManagerBuiltIns; 

ManagerBuiltIns *initBuiltIns(); 
int isBuiltIn(ManagerBuiltIns *manager, char *name);
int executeBuiltIn(ManagerBuiltIns *manager, char **args); 
void freeBuiltIns(ManagerBuiltIns *manager); 

#endif