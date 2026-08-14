#ifndef COMMAND_H
#define COMMAND_H

#include"linked_list.h"

#define MAX_ARGS 1024

typedef struct command {
    char *args[MAX_ARGS];
    int numArgs;
    List *redirections;
} Command;

Command *commandCreate();
void freeCommand(void *data);
void printCommand(void *data); 

#endif
