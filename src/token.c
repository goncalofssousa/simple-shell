#include "token.h"
#include <stdlib.h>

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fd){
    Token *token = malloc(sizeof(Token));

    if (!token) return NULL;

    token->type = type;
    token->value = value;
    token->fd = fd;
    token->redir_type = redir_type;
    token->next = NULL;

    return token;
}

void tokenAppend(Token **head, Token **tail, Token *token) {
    if (!token) return;

    if (*head == NULL) {
        *head = token;
        *tail = token;
    }
    else {
        (*tail)->next = token;
        *tail = token;
    }
}


void tokenFreeList(Token *tokens) {
    Token *next;

    while (tokens) {
        next = tokens->next;
        free(tokens);
        tokens = next;
    }
}