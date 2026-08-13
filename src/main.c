#include<utils.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/types.h>  
#include<tokenizer.h>

int main(){
    print_banner(); 

    char buffer[BUFF_SIZE];

    int result = 0; 
    while(!result){
        write(1, "SaloShell> ", 12);   
        result = readCommandToExecute(buffer); 
        if(result < 0){
            printf("Error on read\n");
            return 1; 
        } else if (result == 0){
            Token *head = tokenizeCommand(buffer); 
            printTokens(head);
        } 

    }

    return 0; 
}