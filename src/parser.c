#include"parser.h"
#include<stdlib.h>

#include <stdio.h>

Command *appendCommand(Command **head, Command **tail){
    Command *new = malloc(sizeof(Command)); 
    if(!new) return NULL; 
    new->red_head = NULL; 
    new->next = NULL;
    new->numArgs = 0; 

    if(*head == NULL){
        *head = new; 
    } else {
        (*tail)->next = new; 
    }
    *tail = new; 
    return new; 
}

void freeRedirectionList(Redirection *redirections){
    Redirection *next;

    while (redirections) {
        next = redirections->next;
        free(redirections);
        redirections = next;
    }
}

void freeComandList(Command *commands) {
    Command *next;

    while (commands) {
        freeRedirectionList(commands->red_head);
        next = commands->next;
        free(commands);
        commands = next;
    }
}


Command *parseTokenList(Token *tokenList){
    Command *head = NULL; 
    Command *tail = NULL; 
    while(tokenList != NULL){
        Command *new = appendCommand(&head, &tail); 
        Redirection *red_tail = NULL;
        while(tokenList != NULL && tokenList->type != TOKEN_PIPE){
            if(tokenList->type == TOKEN_WORD){
                new->args[new->numArgs++] = tokenList->value; 
            } 
            else if (tokenList->type == TOKEN_DIR){
                Redirection *newRedir = malloc(sizeof(Redirection));
                if(!new) return NULL; 
                newRedir->fd = tokenList->fd; 
                newRedir->type = tokenList->redir_type;  
                if(!tokenList->next || tokenList->next->type != TOKEN_WORD){
                    freeComandList(head);
                    free(newRedir); 
                    return NULL; 
                }

                tokenList = tokenList->next;
                newRedir->destFile = tokenList->value; 

                if(new->red_head == NULL){
                    new->red_head = newRedir; 
                } else {
                    red_tail->next = newRedir; 
                }
                red_tail = newRedir; 
            } 

            tokenList = tokenList->next; 
        }
        if(tokenList) tokenList = tokenList->next; 
    }   
    return head; 
}

