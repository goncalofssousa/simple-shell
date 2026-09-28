#include "utils.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

char *initShell() {
    char *username = getenv("USER");
    char hostname[65];

    if (gethostname(hostname, sizeof(hostname)) != 0)
        strcpy(hostname, "unknown");

    hostname[sizeof(hostname) - 1] = '\0';

    printf("\033[H\033[J");

    printf("\033[1;34m");
    printf("==================================================\n");
    printf("\033[1;37m");
    printf("                 SALO SHELL\n");
    printf("\033[1;34m");
    printf("==================================================\n");
    printf("\033[0m");

    printf("\033[1;34mUser:\033[0m     %s\n", username ? username : "unknown");
    printf("\033[1;34mHostname:\033[0m %s\n", hostname);
    printf("\033[1;34mShell:\033[0m    SaloShell\n");

    printf("\033[1;34m==================================================\033[0m\n");
    printf("\n");

    return username;
}

char *readInput(char *path_buffer, int maxSize, char *username, char *home) {
    if (getcwd(path_buffer, maxSize) == NULL) {
        printf("Error getting path\n");
        return NULL;
    }

    char path[PATH_MAX];

    if (home != NULL) {
        size_t homeLen = strlen(home);

        if (strncmp(path_buffer, home, homeLen) == 0 &&
            (path_buffer[homeLen] == '\0' || path_buffer[homeLen] == '/')) {

            snprintf(path, sizeof(path), "~%s", path_buffer + homeLen);
        } else {
            snprintf(path, sizeof(path), "%s", path_buffer);
        }
    } else {
        snprintf(path, sizeof(path), "%s", path_buffer);
    }

    char prompt[PATH_MAX + 128];

    snprintf(prompt, sizeof(prompt),
        "\001\033[1;32m\002%s@%s"
        "\001\033[0m\002:"
        "\001\033[1;34m\002%s"
        "\001\033[0m\002"
        "\001\033[1;37m\002$ "
        "\001\033[0m\002",
        username ? username : "user",
        getenv("HOSTNAME") ? getenv("HOSTNAME") : "localhost",
        path
    );

    char *input = readline(prompt);

    if (input && *input)
        add_history(input);

    return input;
}