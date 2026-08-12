#include<parser.h>
#include<utils.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>



int main(){
    print_banner(); 
    char buffer[BUFF_SIZE];
    int result = 0; 
    while(!result){
        printf("\n"); 
        write(1, "SaloShell> ", 11);   
        result = readCommandToExecute(buffer); 

        if(result == -1){
            printf("Error on read\n"); 
            return 1; 
        } else if(!result) {
            // execução de comando
            Command *command = parseCommand(buffer); 

            printf("Nome do comando: %s\n", command->path);

            printf("Args:\n");
            for (int i = 0; command->args[i] != NULL; i++)
            {
                printf("  [%d] = \"%s\"\n", i, command->args[i]);
            }
            printf("  NULL\n");
            // exit no filho 
            // comunicação via pipe entre os processos para mostrar output?
        }

    }

    return 0; 
}