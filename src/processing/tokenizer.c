#include "processing/tokenizer.h"
#include "entities/token.h"
#include "entities/string-builder.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *getRedirectionStart(char *p) {
    if (*p == '<' || *p == '>') return p;

    if (isdigit(*p)) {
        while (isdigit(*p))
            p++;

        if (*p == '<' || *p == '>') return p;
    }

    return NULL;
}

int getSrcFd(char *command, char *op) {
    if (command == op) return (*op == '<') ? 0 : 1;

    int fd = 0;

    while (command != op) {
        fd = fd * 10 + (*command - '0');
        command++;
    }

    return fd;
}

int expandVariable(StringBuilder *builder, char **command) {
    char *varStart = *command + 1;

    if (!isalnum(*varStart) && *varStart != '_') return 0;

    char *varEnd = varStart;

    while (isalnum(*varEnd) || *varEnd == '_') varEnd++;

    int varLen = varEnd - varStart;

    char *varName = malloc(varLen + 1);

    if (!varName) return -1;

    strncpy(varName, varStart, varLen);
    varName[varLen] = '\0';

    char *value = getenv(varName);

    free(varName);

    if (value) {
        if (builderAppendString(builder, value) == -1) return -1;
    }

    *command = varEnd;

    return 1;
}

int expandTilde(StringBuilder *builder, char **command, int wordStart) {
    if (!wordStart) return 0;

    if ((*command)[1] != '\0' && (*command)[1] != '/') {
        return 0;
    }

    char *home = getenv("HOME");

    if (!home) return 0;

    if (builderAppendString(builder, home) == -1) return -1;

    (*command)++;

    return 1;
}

int addWordToken(List *tokens, StringBuilder *builder) {
    if (builder->length == 0) return 0;

    Token *token = tokenCreate(TOKEN_WORD,builder->data,NONE,-1,-1);

    if (!token) return -1;

    listAppend(tokens, token);

    builder->length = 0;
    builder->data[0] = '\0';

    return 0;
}


int addToken(List *tokens, TokenType type, char *value, RedirectType redir_type, int fdSrc, int fdDest) {
    Token *token = tokenCreate(type,value, redir_type, fdSrc, fdDest);

    if (!token) return -1;

    listAppend(tokens, token);

    return 0;
}


List *tokenizeCommand(char *command) {
    List *tokens = newList();

    if (!tokens) return NULL;

    StringBuilder builder;

    if (builderInit(&builder) == -1) {
        freeList(tokens, freeToken);
        return NULL;
    }

    char quote = '\0';

    int wordStart = 1;

    while (*command != '\0') {

        if (*command == '\'' || *command == '"') {
            if (quote == '\0') quote = *command;
            
            else if (*command == quote) quote = '\0';

            else {
                if (builderAppendChar(&builder,*command) == -1) {
                    free(builder.data);
                    freeList(tokens, freeToken);
                    return NULL;
                }
            }

            command++;
            wordStart = 0;

            continue;
        }

        if (*command == '$' && quote != '\'') {
            int expanded = expandVariable(&builder, &command);

            if (expanded == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            if (expanded) {
                wordStart = 0;
                continue;
            }
        }

        if (*command == '~' && quote == '\0') {
            int expanded = expandTilde(&builder,&command, wordStart);

            if (expanded == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            if (expanded) {
                wordStart = 0;
                continue;
            }
        }

        if (*command == ' ') {
            if (addWordToken(tokens,&builder) == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            command++;

            while (*command == ' ') command++;

            wordStart = 1;

            continue;
        }


        if (*command == '|') {

            if (addWordToken(tokens, &builder) == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            if (addToken(tokens,TOKEN_PIPE,"|",REDIR_INPUT,-1,-1) == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            command++;

            while (*command == ' ') command++;

            wordStart = 1;

            continue;
        }

        char *redir_op = getRedirectionStart(command);
        if (redir_op != NULL) {
            if (addWordToken(tokens, &builder) == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            int fdSrc = getSrcFd(command, redir_op);
            int fdDest = -1;

            RedirectType redir_type = getRedirectType(redir_op);
            int opLen = getOperatorLength(&redir_type, redir_op, &fdDest);

            if (opLen == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            if (addToken(tokens,TOKEN_DIR,"",redir_type,fdSrc,fdDest) == -1) {
                free(builder.data);
                freeList(tokens, freeToken);
                return NULL;
            }

            command = redir_op + opLen;

            while (*command == ' ') command++;

            wordStart = 1;

            continue;
        }

        if (builderAppendChar(&builder,*command) == -1) {
            free(builder.data);
            freeList(tokens, freeToken);
            return NULL;
        }

        command++;
        wordStart = 0;
    }

    if (quote != '\0') {
        free(builder.data);
        freeList(tokens, freeToken);
        printf("Simple Shell: unfinished quote\n"); 
        return NULL;
    }

    if (addWordToken(tokens,&builder) == -1) {
        free(builder.data);
        freeList(tokens, freeToken);
        return NULL;
    }

    free(builder.data);

    return tokens;
}