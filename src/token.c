#include "token.h"
#include <stdlib.h>

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fd) {
    Token *token = malloc(sizeof(Token));

    if (!token) return NULL;

    token->type = type;
    token->value = value;
    token->fd = fd;
    token->redir_type = redir_type;
    token->next = NULL;

    return token;
}

void freeToken(void *data){
    Token *token = (Token *) data; 

    if(!token) return; 

    free(token); 
}
