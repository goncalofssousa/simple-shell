#ifndef TOKEN_H
#define TOKEN_H

#include"redirection.h"

typedef enum token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_DIR
} TokenType;


typedef struct token {
    TokenType       type;
    char            *value;
    int             fd;
    RedirectType    redir_type;
    struct token    *next;
} Token;

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fd);
void freeToken(void *data);

#endif