#include<stdlib.h>
#include<parser.h>
#include<string.h>


Command *parseCommand(char *command) {
    Command *result = malloc(sizeof(Command));

    if (result == NULL)
        return NULL;

    while (*command == ' ') command++;

    result->path = strsep(&command, " ");

    if (result->path == NULL || *result->path == '\0'){
        free(result);
        return NULL;
    }

    result->args[0] = result->path;
    result->numArgs = 1;

    if (command != NULL){
        while (*command == ' ') command++;

        char quote = '\0';
        char *ptr = command;

        while (*command != '\0'){
            if (*command == '"' || *command == '\''){
                if (quote == '\0') {
                    quote = *command;
                    ptr = command + 1;
                }
                else if (*command == quote) {
                    quote = '\0';

                    result->args[result->numArgs++] = ptr;

                    *command = '\0';
                    ptr = command + 1;
                }

                command++;
            } else if (*command == ' ' && quote == '\0') {
                *command = '\0';

                if (*ptr != '\0') result->args[result->numArgs++] = ptr;

                command++;

                while (*command == ' ') command++;

                ptr = command;
            } else {
                command++;
            }
        }

        if (quote != '\0')
        {
            free(result);
            return NULL;
        }

        while (*ptr == ' ') ptr++;

        if (*ptr != '\0') result->args[result->numArgs++] = ptr;
    }

    result->args[result->numArgs] = NULL;

    return result;
}