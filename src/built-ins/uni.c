#include "built-ins/uni.h"
#include <stdio.h>
#include <unistd.h>

int uni(char **args) {
    if(chdir("/home/goncalo/Documents/Universidade/3ano/1sem/") < 0){
        printf("cd: No such file or directory | Built in\n");
        return 1;
    } 
    if (chdir(args[1]) < 0) {
        printf("cd: No such file or directory\n");
        return 1;
    }

    return 0;
}
