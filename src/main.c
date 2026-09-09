#include "utils.h"

#include "processing/tokenizer.h"
#include "processing/parser.h"
#include "processing/executer.h"

#include "data-structures/linked_list.h"

#include "entities/command.h"
#include "entities/token.h"

#include "built-ins/manager-builtIns.h"

#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/types.h>  
#include <linux/limits.h>

int main() {
    char *home = getenv("HOME");
    char *user = initShell();
    ManagerBuiltIns *managerBuiltIns = initBuiltIns();

    char path_buffer[PATH_MAX];

    while (1) {
        char *input = readInput(path_buffer, sizeof(path_buffer), user, home);

        if (!input) break;

        List *tokensList = tokenizeCommand(input);

        if (!tokensList) {
            free(input);
            continue;
        }

        List *commandsList = parseTokenList(tokensList);
        freeList(tokensList, freeToken);

        if (!commandsList) {
            free(input);
            continue;
        }

        execute(managerBuiltIns, commandsList);

        freeList(commandsList, freeCommand);
        free(input);
    }

    freeBuiltIns(managerBuiltIns);

    return 0;
}
