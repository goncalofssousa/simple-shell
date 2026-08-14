#include "command.h"
#include <stdlib.h>
#include<stdio.h>
#include "linked_list.h"
#include "redirection.h"

Command *commandCreate() {
    Command *new = malloc(sizeof(Command));
    if (!new) return NULL;

    new->redirections = newList();
    new->numArgs = 0;

    return new;
}

void freeCommand(void *data) {
    Command *cmd = (Command *) data; 

    if(!cmd) return; 

    freeList(cmd->redirections, freeRedirection);
    free(cmd);
}

void printCommand(void *data) {
    Command *cmd = (Command *) data; 
    if(!cmd) return; 

    printf("Command: %s\n", cmd->args[0]); 

    printf("Args:\n"); 
    for(int i = 0; i < cmd->numArgs; i++){
        printf("   %d: %s\n", i, cmd->args[i]); 
    }

    printf("Redirections:\n"); 
    printList(cmd->redirections, printRedirection); 

    printf("\n"); 
}

