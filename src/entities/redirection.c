#include "entities/redirection.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


char *redir_type_name(RedirectType type) {
    switch (type) {
        case REDIR_INPUT:   return "REDIR_INPUT";
        case REDIR_OUTPUT:  return "REDIR_OUTPUT";
        case REDIR_APPEND:  return "REDIR_APPEND";
        case REDIR_HEREDOC: return "REDIR_HEREDOC";
        case REDIR_DUPLICATE: return "REDIR_DUP";
        case REDIR_INOUT:   return "REDIR_INOUT";
        case REDIR_CLOSE:   return "REDIR_CLOSE";
        case NONE: return "REDIR_NONE";
    }
    return "UNKNOWN";
}

int getOperatorLength(RedirectType *redir_type,char *redir_op,int *fdDest) {
    if (*redir_type != REDIR_DUPLICATE) {
        return (*redir_type == REDIR_APPEND || *redir_type == REDIR_HEREDOC || *redir_type == REDIR_INOUT) ? 2 : 1;
    }

    int op_len = 2;

    redir_op += 2;

    if (!isdigit(*redir_op) && *redir_op != '-') {
        return -1;
    }

    int numLen = 0;

    if (*redir_op == '-') {
        *fdDest = -1;
        *redir_type = REDIR_CLOSE;
        numLen = 1;

    } else {

        *fdDest = 0;

        while (isdigit(*redir_op)) {
            *fdDest = *fdDest * 10 + (*redir_op - '0');

            redir_op++;
            numLen++;
        }
    }

    return op_len + numLen;
}


Redirection *redirectionCreate(int fdSrc, int fdDest, RedirectType type, char *destFile) {
    Redirection *redir = malloc(sizeof(Redirection));
    if (!redir) return NULL;

    redir->fdSrc = fdSrc;
    redir->fdDest = fdDest;
    redir->type = type;
    redir->destFile = strdup(destFile);
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
    printf("type: %s | fdSrc: %d | fdDest: %d | destFile: %s\n", redir_type_name(red->type), red->fdSrc, red->fdDest, red->destFile);

}

RedirectType getRedirectType(char *op) {
    if (op[0] == '<' && op[1] == '>') 
        return REDIR_INOUT;

    if (op[1] == '<')
        return REDIR_HEREDOC;

    if (op[1] == '>')
        return REDIR_APPEND;
    
    if (op[1] == '&')
        return REDIR_DUPLICATE;
    
    if (op[0] == '<')
        return REDIR_INPUT;
        
    return REDIR_OUTPUT;
}

