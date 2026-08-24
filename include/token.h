#ifndef TOKEN_H
#define TOKEN_H

#include"redirection.h"

typedef enum token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_DIR
} TokenType;


typedef struct token {
    TokenType type;
    char *value;
    int fdSrc;
    int fdDest; 
    RedirectType redir_type;
    struct token *next;
} Token;

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fdSrc, int fdDest);
void freeToken(void *data);
void printToken(void *data);

#endif