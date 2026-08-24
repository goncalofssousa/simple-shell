#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "linked_list.h"

char *getRedirectionStart(char *p) {
    if (*p == '<' || *p == '>')
        return p;

    if (isdigit(*p))
    {
        while (isdigit(*p))
            p++;
        if (*p == '<' || *p == '>')
            return p;
    }

    return NULL;
}

int getSrcFd(char *command, char *op) {
    int fd;

    if (command == op) return (*op == '<') ? 0 : 1;

    fd = 0;
    while (command != op) {
        fd = fd * 10 + (*command - '0');
        command++;
    }
    return fd;
}

int getOperatorLength(RedirectType *redir_type, char *redir_op, int *fdDest) {
    int op_len = 0;    
    if(*redir_type == REDIR_DUPLICATE){
        op_len = 2;

        redir_op += op_len;
        
        if(!isdigit(*redir_op) && *redir_op != '-'){ 
            return -1; 
        }

        int numlen = 0; 

        if(*redir_op == '-') {
            *fdDest = -1; 
            *redir_type = REDIR_CLOSE;
            numlen++; 
        } else {
            *fdDest = 0; 
            while(isdigit(*redir_op)){
                *fdDest = *fdDest * 10 + (*redir_op - '0'); 
                redir_op++;
                numlen++; 
            }
        }

        op_len += numlen;
    }
    else  op_len = (*redir_type == REDIR_APPEND || *redir_type == REDIR_HEREDOC || *redir_type == REDIR_INOUT) ? 2 : 1;

    return op_len;
}

int addWordToken(List *tokens, char *start, char *end) {
    Token *newToken;

    if (start == end || *start == '\0') return 0;

    newToken = tokenCreate(TOKEN_WORD, start, NONE, -1, -1);
    if (!newToken){
        freeList(tokens, freeToken);
        return -1;
    }
    listAppend(tokens, newToken);
    return 0;
}

int addToken(List *tokens, TokenType type, char *value, RedirectType redir_type, int fdSrc, int fdDest) {
    Token *newToken = tokenCreate(type, value, redir_type, fdSrc, fdDest);

    if (!newToken) {
        freeList(tokens, freeToken); 
        return -1;
    }
    listAppend(tokens, newToken); 
    return 0;
}



List *tokenizeCommand(char *command) {
    char *ptr = command;
    char quote = '\0';
    List *tokens = newList(); 
    char *redir_op;

    while (*command != '\0') {
        if (*command == '\'' || *command == '"') {
            if (quote == '\0') quote = *command;
            else if (*command == quote) {
                *command = '\0';
                if (addWordToken(tokens, ptr, command) == -1) return NULL;
                quote = '\0';
            }
            ptr = command + 1;
            command++;
            continue;
        }

        if (quote != '\0') {
            command++;
            continue;
        }

        if (*command == ' ') {
            *command = '\0';
            if (addWordToken(tokens, ptr, command) == -1) return NULL;

            command++;
            while (*command == ' ') command++;
            ptr = command;
            continue;
        }

        if (*command == '|') {
            *command = '\0';
            if (addWordToken(tokens, ptr, command) == -1) return NULL;
            if (addToken(tokens, TOKEN_PIPE, "|", REDIR_INPUT, -1, -1) == -1) return NULL;

            command++;
            while (*command == ' ') command++;
            ptr = command;
            continue;
        }

        redir_op = getRedirectionStart(command);
        if (redir_op != NULL) {

            int fdSrc = getSrcFd(command, redir_op);
            int fdDest = -1; 

            RedirectType redir_type = getRedirectType(redir_op);

            int op_len = getOperatorLength(&redir_type, redir_op, &fdDest);
            if(op_len == -1){
                freeList(tokens, freeToken);
                return NULL;
            }
        
            *command = '\0';
            if (addWordToken(tokens, ptr, command) == -1) {
                freeList(tokens, freeToken);
                return NULL;
            }
            if (addToken(tokens, TOKEN_DIR, "\0", redir_type, fdSrc, fdDest) == -1) {
                freeList(tokens, freeToken);
                return NULL;
            }

            command = redir_op + op_len;
            while (*command == ' ') command++;
            ptr = command;
            continue;
        }

        command++;
    }

    if (addWordToken(tokens, ptr, command) == -1) return NULL;

    return tokens;
}