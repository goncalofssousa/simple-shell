#ifndef PARSER_H
#define PARSER_H

typedef struct command {
    char *path;
    char *args[1024];
    int numArgs;
} Command;

Command *parseCommand(char *command);

#endif