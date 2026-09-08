#include "built-ins/cd.h"
#include <stdio.h>
#include <stdlib.h>

int cd(char **args){
    if(args[1] == NULL){
        printf("Cd sem args\n"); 
    } else {
        printf("Cd com args\n"); 
    }
    return 1; 
}