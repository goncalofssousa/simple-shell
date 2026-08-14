#include"redirection.h"
#include<stdlib.h>
#include<stdio.h>


char *redir_type_name(RedirectType type) {
    switch (type) {
        case REDIR_INPUT:   return "REDIR_INPUT (<)";
        case REDIR_OUTPUT:  return "REDIR_OUTPUT (>)";
        case REDIR_APPEND:  return "REDIR_APPEND (>>)";
        case REDIR_HEREDOC: return "REDIR_HEREDOC (<<)";
    }
    return "UNKNOWN";
}

Redirection *redirectionCreate(int fd, RedirectType type, char *destFile) {
    Redirection *redir = malloc(sizeof(Redirection));
    if (!redir) return NULL;

    redir->fd = fd;
    redir->type = type;
    redir->destFile = destFile;
    redir->next = NULL;

    return redir;
}

void freeRedirection(void *data){
    Redirection *redir = (Redirection *) data; 
    if(!redir) return; 
    free(redir); 
}

void printRedirection(void *data){
    Redirection *red = (Redirection *) data; 
    if(!red) return; 
    printf("type: %s | fd: %d | destFile: %s\n", redir_type_name(red->type), red->fd, red->destFile);

}

