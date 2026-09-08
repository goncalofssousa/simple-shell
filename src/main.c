#include "utils.h"

#include "processing/tokenizer.h"
#include "processing/parser.h"
#include "processing/executer.h"

#include "data-structures/linked_list.h"

#include "entities/command.h"
#include "entities/token.h"

#include "built-ins/manager-builtIns.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/types.h>  
#include <linux/limits.h>

int main(){
    printBanner();
    ManagerBuiltIns *managerBuiltIns = initBuiltIns(); 

    char path[PATH_MAX];
    char *input; 

    while(1){
        input = readInput(path, sizeof(path)); 
        if(!input) break;

        List *tokensList = tokenizeCommand(input);

        List *commandsList = parseTokenList(tokensList);
        freeList(tokensList, freeToken); 


        execute(managerBuiltIns, commandsList);
        freeList(commandsList, freeCommand);  
         
        free(input); 
    }

    freeBuiltIns(managerBuiltIns); 
    return 0; 
}
