#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<utils.h>

int readCommandToExecute(char *buffer){
    int bytesRead = read(0, buffer, sizeof buffer[0] * BUFF_SIZE - 1);
    if(bytesRead <= 0){
        return -1;
    } else {
        buffer[bytesRead - 1] = '\0';
        if((strcmp(buffer, "quit")) == 0) return 1;
        return 0; 
    }
}

void print_banner(){
    printf("\033[H\033[J"); 

    printf("========================================\n");
    printf("              SaloShell\n");
    printf("========================================\n");
    printf("  A simple Unix shell written in C\n");
    printf("  Type 'help' for available commands.\n");
    printf("========================================\n\n");
}