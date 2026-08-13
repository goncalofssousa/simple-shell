#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "token.h"

Token *tokenizeCommand(char *command);
void printTokens(Token *tokens);
char *redir_type_name(RedirectType type);
#endif