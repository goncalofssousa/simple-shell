#include<unistd.h>
#include<linux/limits.h>


void builtInPwd(){
    char path[PATH_MAX]; 

    if(getcwd(path, sizeof(path)) == NULL){
        printf("Invalid Path\n"); 
        return 1; 
    }   

    printf("%s\n", path); 
}