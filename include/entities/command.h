#ifndef COMMAND_H
#define COMMAND_H

#include "data-structures/linked_list.h"

#define MAX_ARGS 1024

typedef struct command {
    char *args[MAX_ARGS];
    int numArgs;
    List *redirections;
} Command;

Command *commandCreate();
int appendArg(Command *cmd, char *arg); 
void freeCommand(void *data);
void printCommand(void *data); 

#endif
