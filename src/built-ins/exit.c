#include "built-ins/exit.h"
#include <unistd.h>
#include <stdlib.h>

int builtin_exit(char **args) {
    int status = 0;

    if (args[1] != NULL){
        status = atoi(args[1]);
    }

    exit(status);
    return 0; 
}