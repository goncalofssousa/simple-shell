#include "entities/builtIn.h"
#include "built-ins/cd.h"
#include "built-ins/manager-builtIns.h"

ManagerBuiltIns *initBuiltIns(){
    ManagerBuiltIns *manager = malloc(sizeof(ManagerBuiltIns)); 
    manager->builtIns =g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify) freeBuiltIn);

    BuiltIn *builtInCd = createBuiltIn("cd", cd); 
    g_hash_table_insert(manager->builtIns, "cd", builtInCd);

    return manager; 
}

int isBuiltIn(ManagerBuiltIns *manager, char *name){
    gboolean result = g_hash_table_contains(manager->builtIns, name);
    if(result == TRUE) return 1; 
    return 0;   
}

int executeBuiltIn(ManagerBuiltIns *manager, char **args){
    gpointer builtIn = g_hash_table_lookup(manager->builtIns, args[0]); 
    if(builtIn == NULL) return 1;
    
    BuiltIn *toExecute = (BuiltIn *) builtIn; 
    int result = toExecute->execution(args);
    return result; 
}

void freeBuiltIns(ManagerBuiltIns *manager){
    g_hash_table_destroy(manager->builtIns); 
    free(manager); 
}