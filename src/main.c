#include<utils.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <sys/types.h>  
#include<tokenizer.h>
#include<parser.h>

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
            Token *tokensHead = tokenizeCommand(buffer); 
            Command *commandsHead = parseTokenList(tokensHead);
            tokenFreeList(tokensHead); 
            while(commandsHead != NULL){
                printf("Command: %s\n", commandsHead->args[0]); 
                printf("Args:\n"); 
                for(int i = 0; i < commandsHead->numArgs; i++){
                    printf("   %d: %s\n", i, commandsHead->args[i]); 
                }
                printf("Redirections:\n"); 
                Redirection *red = commandsHead->red_head; 
                while(red != NULL){
                    printf("type: %s | fd: %d | destFile: %s\n", redir_type_name(red->type), red->fd, red->destFile);
                    red = red->next; 
                }
                printf("\n"); 
                commandsHead = commandsHead->next; 
            }

            freeComandList(commandsHead); 
        } 


    }

    return 0; 
}