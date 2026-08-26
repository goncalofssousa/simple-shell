#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<utils.h>
#include<linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<stdlib.h>

char *readInput(char *buffer, int maxSize) {
    if (getcwd(buffer, maxSize) == NULL) {
        printf("Error reading path\n");
        return NULL;
    }

    char prompt[PATH_MAX + 128];
    char *username = getenv("USER");


    snprintf(prompt, sizeof(prompt),
            "\001\033[1;32m\002%s"
            "\001\033[0m\002:"
            "\001\033[1;34m\002%s"
            "\001\033[0m\002$ ",
            username, buffer);

    char *input = readline(prompt);

    if (input && *input) add_history(input);

    return input;
}

void clear(){
    printf("\033[H\033[J"); 
}