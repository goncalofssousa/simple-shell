#ifndef REDIRECTION_H
#define REDIRECTION_H

typedef enum redirect_type {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_INOUT,
    REDIR_APPEND,
    REDIR_DUPLICATE,
    REDIR_HEREDOC,
    REDIR_CLOSE,
    NONE
} RedirectType;

typedef struct redirection {
    int fdSrc;
    int fdDest; 
    char *destFile;
    RedirectType type;
    struct redirection *next;
} Redirection;


Redirection *redirectionCreate(int fdSrc, int fdDest, RedirectType type, char *destFile); 
void freeRedirection(void *data); 
void printRedirection(void *data);
RedirectType getRedirectType(char *op);
char *redir_type_name(RedirectType type);

#endif