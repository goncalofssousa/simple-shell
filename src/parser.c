#include "parser.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void resolveHeredoc(char *filename, char *delimiter) {
    char buffer[1024];

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) return;

    while (1) {
        write(STDOUT_FILENO, "> ", 2);

        int bytesRead = read(0, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0)break;

        buffer[bytesRead] = '\0';

        if (buffer[bytesRead - 1] == '\n') {
            buffer[bytesRead - 1] = '\0';

            if (strcmp(buffer, delimiter) == 0) break;

            buffer[bytesRead - 1] = '\n';
        }

        write(fd, buffer, bytesRead);
    }

    close(fd);
}


int isInvalid(ListNode *node, RedirectType type) {
    if (type == REDIR_CLOSE || type == REDIR_DUPLICATE)
        return 0;

    if (node->next == NULL)
        return 1;

    Token *nextToken = node->next->data;

    return nextToken == NULL || nextToken->type != TOKEN_WORD;
}


char *createHeredocFile(int numHeredoc) {
    char fileTmp[64];

    snprintf(fileTmp, sizeof(fileTmp), "tmp/heredoc_%d", numHeredoc);

    char *filename = malloc(strlen(fileTmp) + 1);
    if (!filename) return NULL;

    strcpy(filename, fileTmp);

    return filename;
}


Redirection *createRedirection(ListNode *node, int *numHeredoc) {
    Token *token = node->data;

    if (isInvalid(node, token->redir_type)) return NULL;

    if (token->redir_type == REDIR_CLOSE || token->redir_type == REDIR_DUPLICATE) {

        return redirectionCreate(token->fdSrc, token->fdDest, token->redir_type, NULL);
    }

    Token *nextToken = node->next->data;

    if (token->redir_type == REDIR_HEREDOC) {
        (*numHeredoc)++;

        char *filename = createHeredocFile(*numHeredoc);
        if (!filename) return NULL;

        resolveHeredoc(filename, nextToken->value);

        Redirection *redir = redirectionCreate(token->fdSrc,token->fdDest,token->redir_type,filename);

        if (!redir) free(filename);

        return redir;
    }

    return redirectionCreate(token->fdSrc,token->fdDest,token->redir_type,nextToken->value);
}


List *parseTokenList(List *tokenList) {
    List *commands = newList();
    if (!commands) return NULL;

    ListNode *current = tokenList->head;
    int numHeredoc = 0;

    while (current) {
        Command *cmd = commandCreate();
        if (!cmd) {
            freeList(commands, freeCommand);
            return NULL;
        }

        listAppend(commands, cmd);

        while (current) {
            Token *token = current->data;

            if (!token) {
                freeList(commands, freeCommand);
                return NULL;
            }

            if (token->type == TOKEN_WORD) {
                cmd->args[cmd->numArgs++] = token->value;
            } else if (token->type == TOKEN_DIR) {
                Redirection *redir = createRedirection(current, &numHeredoc);

                if (!redir) {
                    freeList(commands, freeCommand);
                    return NULL;
                }

                listAppend(cmd->redirections, redir);

                if (token->redir_type != REDIR_CLOSE && token->redir_type != REDIR_DUPLICATE) {
                    current = current->next;
                }
            } else break; 

            current = current->next;
        }

        cmd->args[cmd->numArgs] = NULL;

        if (current) current = current->next;
    }

    return commands;
}