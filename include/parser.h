#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct redirection {
    int fd;
    char *destFile; 
    RedirectType type; 
    struct redirection *next;
} Redirection; 

typedef struct command {
    char *args[1024]; 
    int numArgs; 
    Redirection *red_head;
    struct command *next;  
} Command; 


Command *parseTokenList(Token *tokenList);
void freeComandList(Command *commands);

#endif