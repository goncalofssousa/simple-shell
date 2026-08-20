#ifndef REDIRECTION_H
#define REDIRECTION_H

typedef enum redirect_type {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} RedirectType;

typedef struct redirection {
    int fd;
    char *destFile;
    RedirectType type;
    struct redirection *next;
} Redirection;


Redirection *redirectionCreate(int fd, RedirectType type, char *destFile); 
void freeRedirection(void *data); 
void printRedirection(void *data);

#endif