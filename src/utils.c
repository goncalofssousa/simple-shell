#include "utils.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

void printBanner() {
    char hostname[65];
    char *username = getenv("USER");

    if (gethostname(hostname, sizeof(hostname)) != 0)
        strcpy(hostname, "unknown");

    hostname[sizeof(hostname) - 1] = '\0';

    printf("\033[H\033[J");

    // Vermelho — decoração
    printf("\033[1;31m");
    printf("==================================================\n");
    printf("\033[1;37m");
    printf("                 SALO SHELL\n");
    printf("\033[1;31m");
    printf("==================================================\n");
    printf("\033[0m");

    printf("\033[1;31mUser:\033[0m     %s\n",
           username ? username : "unknown");

    printf("\033[1;31mHostname:\033[0m %s\n", hostname);

    printf("\033[1;31mShell:\033[0m    SaloShell\n");

    printf("\033[1;31m==================================================\033[0m\n");
    printf("\n");
}

char *readInput(char *buffer, int maxSize) {
    if (getcwd(buffer, maxSize) == NULL) {
        printf("Error reading path\n");
        return NULL;
    }

    char *username = getenv("USER");
    if (username == NULL)
        username = "user";

    char prompt[PATH_MAX + 128];

    snprintf(prompt, sizeof(prompt),
        "\001\033[1;31m\002%s"
        "\001\033[0m\002@%s"
        "\001\033[1;31m\002> "
        "\001\033[0m\002",
        username,
        buffer
    );

    char *input = readline(prompt);

    if (input && *input)
        add_history(input);

    return input;
}