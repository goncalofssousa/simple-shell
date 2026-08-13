#ifndef TOKEN_H
#define TOKEN_H

typedef enum token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_DIR
} TokenType;

typedef enum redirect_type {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} RedirectType;

typedef struct token {
    TokenType       type;
    char            *value;
    int             fd;
    RedirectType    redir_type;
    struct token    *next;
} Token;

Token *tokenCreate(TokenType type, char *value, RedirectType redir_type, int fd);
void tokenAppend(Token **head, Token **tail, Token *token);
void tokenFreeList(Token *tokens);

#endif