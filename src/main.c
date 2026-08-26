#include<utils.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/types.h>  
#include<tokenizer.h>
#include<command.h>
#include<parser.h>
#include<execution.h>
#include"linked_list.h"
#include<linux/limits.h>

int main(){
    clear(); 

    char path[PATH_MAX];
    char *input; 

    while(1){
        input = readInput(path, sizeof(path)); 
        if(!input) break;

        List *tokensList = tokenizeCommand(input);

        List *commandsList = parseTokenList(tokensList);
        freeList(tokensList, freeToken); 


        execute(commandsList);
        freeList(commandsList, freeCommand);  
         
        free(input); 
    }

    return 0; 
}
