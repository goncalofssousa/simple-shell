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
    print_banner(); 

    char path[PATH_MAX];
    char buffer[BUFF_SIZE];

    int result = 0; 
    while(!result){

        if(showPath(path, PATH_MAX)) return 1; 
        
        result = readCommandToExecute(buffer); 
        if (result == 0){
            List *tokensList = tokenizeCommand(buffer);

            List *commandsList = parseTokenList(tokensList);
            freeList(tokensList, freeToken); 


            execute(commandsList);
            freeList(commandsList, freeCommand);  
        } 
        else return 1; 
    }

    return 0; 
}
