#include "entities/command.h"
#include "entities/redirection.h"

#include "data-structures/linked_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    for(int i = 0; i < cmd->numArgs; i++){
        if(cmd->args[i]) free(cmd->args[i]); 
    }
    freeList(cmd->redirections, freeRedirection);
    free(cmd);
}

int appendArg(Command *cmd, char *arg){
    if(cmd == NULL) return 1;

    if(arg == NULL){
        cmd->args[cmd->numArgs] = NULL; 
        return 0; 
    }
    
    cmd->args[cmd->numArgs++] = strdup(arg);
    return 0; 
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

