#include "token.h"
#include <stdlib.h>
#include <stdio.h>

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fdSrc, int fdDest) {
    Token *token = malloc(sizeof(Token));

    if (!token) return NULL;

    token->type = type;
    token->value = value;
    token->fdSrc = fdSrc;
    token->fdDest = fdDest;
    token->redir_type = redir_type;
    token->next = NULL;

    return token;
}

void freeToken(void *data){
    Token *token = (Token *) data; 

    if(!token) return; 

    free(token); 
}

char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_DIR:  return "REDIRECTION";
    }
    return "UNKNOWN";
}

void printToken(void *data){
    Token *token = (Token *) data; 
    if(!token) return; 

    printf("type: %s | value: %s | fdSrc: %d | fdDest: %d | redir_type: %s\n", token_type_name(token->type), token->value, token->fdSrc, token->fdDest, redir_type_name(token->redir_type));
}