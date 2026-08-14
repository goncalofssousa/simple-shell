#include "parser.h"
#include <stdlib.h>
#include<stdio.h>

List *parseTokenList(List *tokenList) {
    List *commands = newList(); 
    if(!commands) return NULL; 
    ListNode *atual = tokenList->head; 
    while (atual != NULL) {
        Command *cmd = commandCreate(); 
        if (!cmd) {
            freeList(commands, freeCommand);
            return NULL;
        }
        listAppend(commands, cmd);

        Token *token = atual->data; 
        if(!token) {
            freeList(commands, freeCommand); 
            return NULL;
        }             

        while (atual != NULL && token->type != TOKEN_PIPE) {
            if (token->type == TOKEN_WORD) {
                cmd->args[cmd->numArgs++] = token->value;
            }
            else if (token->type == TOKEN_DIR) {
                if(atual->next == NULL){
                    freeList(commands, freeCommand); 
                    return NULL;
                }

                Token *nextToken = atual->next->data; 
                if(!nextToken || nextToken->type != TOKEN_WORD) {
                    freeList(commands, freeCommand); 
                    return NULL;
                } 

                Redirection *newRedir = redirectionCreate(token->fd, token->redir_type, nextToken->value);
                if (!newRedir) {
                    freeList(commands, freeCommand); 
                    return NULL;
                }
                listAppend(cmd->redirections, newRedir);

                atual = atual->next;
            }

            atual = atual->next;
            if(atual){
                token = atual->data; 
                if(!token) return NULL; 
            }
        }
        cmd->args[cmd->numArgs] = NULL; 
        if (atual) atual = atual->next;
    }

    return commands;
}
