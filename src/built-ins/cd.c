#include "built-ins/cd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cd(char **args) {
    char *home = getenv("HOME");
    if (args[1] == NULL || (args[1][0] == '~' && args[1][1] == '\0')) {
        if (chdir(home) < 0) {
            printf("cd: No such file or directory\n");
            return 1;
        }
    } else {
        if (chdir(args[1]) < 0) {
            printf("cd: No such file or directory\n");
            return 1;
        }
    }

    return 0;
}
