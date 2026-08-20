#include "parser.h"
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>

void resolveHeredoc(char *filename, char *delimiter){
    int bytesRead;
    char buffer[1024]; 

    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
    while(1){
        write(1, "> ", 2);  
        bytesRead = read(0, buffer, sizeof(buffer) - 1);
        buffer[bytesRead - 1] = '\0'; 
        if(bytesRead <= 0 || (strcmp(buffer, delimiter) == 0)){
            break;
        }
        buffer[bytesRead - 1] = '\n'; 
        write(fd, buffer, bytesRead); 
    }
    close(fd); 
}


List *parseTokenList(List *tokenList) {
    List *commands = newList(); 
    if(!commands) return NULL; 
    ListNode *atual = tokenList->head; 
    int numHeredoc = 0; 
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
                
                Redirection *newRedir = NULL; 

                if(token->redir_type == REDIR_HEREDOC){
                    numHeredoc++; 
                    char fileTmp[64]; 
                    snprintf(fileTmp, sizeof(fileTmp), "tmp/heredoc_%d", numHeredoc); 
                    char *filename = malloc(strlen(fileTmp)); 
                    strcpy(filename, fileTmp); 
                    resolveHeredoc(filename, nextToken->value);
                    newRedir = redirectionCreate(token->fd, token->redir_type, filename);
                }
                else {
                    newRedir = redirectionCreate(token->fd, token->redir_type, nextToken->value);
                }

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
