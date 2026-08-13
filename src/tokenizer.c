#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_DIR:  return "REDIRECTION";
    }
    return "UNKNOWN";
}

char *redir_type_name(RedirectType type) {
    switch (type) {
        case REDIR_INPUT:   return "REDIR_INPUT (<)";
        case REDIR_OUTPUT:  return "REDIR_OUTPUT (>)";
        case REDIR_APPEND:  return "REDIR_APPEND (>>)";
        case REDIR_HEREDOC: return "REDIR_HEREDOC (<<)";
    }
    return "UNKNOWN";
}

void printTokens(Token *tokens) {
    int i = 0;

    while (tokens)
    {
        printf("Token %d:\n", i);
        printf("  type: %s\n", token_type_name(tokens->type));
        printf("  value: %s\n", tokens->value);

        if (tokens->type == TOKEN_DIR)
        {
            printf("  fd: %d\n", tokens->fd);
            printf("  redirect_type: %s\n", redir_type_name(tokens->redir_type));
        }

        printf("\n");
        tokens = tokens->next;
        i++;
    }
}

RedirectType parseRedirectOperator(char *op) {
    if (op[0] == '<' && op[1] == '<')
        return REDIR_HEREDOC;
    if (op[0] == '>' && op[1] == '>')
        return REDIR_APPEND;
    if (op[0] == '<')
        return REDIR_INPUT;
    return REDIR_OUTPUT;
}

char *getRedirectionOperator(char *p) {
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

int parseFd(char *command, char *op) {
    int fd;

    if (command == op) return (*op == '<') ? 0 : 1;

    fd = 0;
    while (command != op)
    {
        fd = fd * 10 + (*command - '0');
        command++;
    }
    return fd;
}

int addWordToken(Token **head, Token **tail, char *start, char *end) {
    Token *newToken;

    if (start == end || *start == '\0')
        return 0;

    newToken = tokenCreate(TOKEN_WORD, start, REDIR_INPUT, -1);
    if (!newToken){
        tokenFreeList(*head);
        *head = NULL;
        *tail = NULL;
        return -1;
    }
    tokenAppend(head, tail, newToken);
    return 0;
}

int addToken(Token **head, Token **tail, TokenType type, char *value, RedirectType redir_type, int fd) {
    Token *newToken = tokenCreate(type, value, redir_type, fd);

    if (!newToken) {
        tokenFreeList(*head);
        *head = NULL;
        *tail = NULL;
        return -1;
    }
    tokenAppend(head, tail, newToken);
    return 0;
}

Token *tokenizeCommand(char *command) {
    char    *ptr = command;
    char    quote = '\0';
    Token   *head = NULL;
    Token   *tail = NULL;
    char    *redir_op;

    while (*command != '\0') {
        if (*command == '\'' || *command == '"') {
            if (quote == '\0')
                quote = *command;
            else if (*command == quote)
            {
                *command = '\0';
                if (addWordToken(&head, &tail, ptr, command) == -1)
                    return NULL;
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
            if (addWordToken(&head, &tail, ptr, command) == -1)
                return NULL;

            command++;
            while (*command == ' ')
                command++;
            ptr = command;
            continue;
        }

        if (*command == '|') {
            *command = '\0';
            if (addWordToken(&head, &tail, ptr, command) == -1)
                return NULL;
            if (addToken(&head, &tail, TOKEN_PIPE, "|", REDIR_INPUT, -1) == -1)
                return NULL;

            command++;
            while (*command == ' ')
                command++;
            ptr = command;
            continue;
        }

        redir_op = getRedirectionOperator(command);
        if (redir_op != NULL) {
            int fd = parseFd(command, redir_op);
            RedirectType redir_type = parseRedirectOperator(redir_op);
            int op_len = (redir_type == REDIR_APPEND || redir_type == REDIR_HEREDOC) ? 2 : 1;

            *command = '\0';
            if (addWordToken(&head, &tail, ptr, command) == -1) return NULL;
            if (addToken(&head, &tail, TOKEN_DIR, "\0", redir_type, fd) == -1)return NULL;

            command = redir_op + op_len;
            while (*command == ' ') command++;
            ptr = command;
            continue;
        }

        command++;
    }

    if (addWordToken(&head, &tail, ptr, command) == -1) return NULL;

    return head;
}