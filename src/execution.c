#include<command.h>
#include<linked_list.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<redirection.h>
#include<string.h>

int handleCommandRedirects(List *redirections){
    ListNode *node = redirections->head; 
    while(node != NULL){
        Redirection *redirection = (Redirection *) node->data; 
        int fd;  
        switch(redirection->type){
            case REDIR_INPUT:
                fd = open(redirection->destFile, O_RDONLY); 
                if(fd < 0) return 1; 
                if(dup2(fd, redirection->fd) == -1) return 1;
                close(fd);
                break; 
            case REDIR_OUTPUT: 
                fd = open(redirection->destFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd < 0) return 1; 
                if(dup2(fd, redirection->fd) == -1) return 1;
                close(fd);
                break;
            case REDIR_APPEND: 
                fd = open(redirection->destFile,  O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(fd < 0) return 1; 
                if(dup2(fd, redirection->fd) == -1) return 1;
                close(fd);
                break;
            case REDIR_HEREDOC: 
                int fdRead = open(redirection->destFile, O_RDONLY); 
                unlink(redirection->destFile);
                dup2(fdRead, redirection->fd); 
                close(fdRead);
                break;
            default: break; 
        }
        node = node->next;  
    }
    return 0; 
}

int execute(List *commands){
    ListNode *node = commands->head; 
    int fd_in = 0; 
    int pipe_fds[2]; 
    int numCommands = 0; 
    while(node != NULL){
        if(node->next != NULL){
            // verificar caso de erro
            pipe(pipe_fds);
        }    
        pid_t pid = fork(); 
        if(pid == 0){
            Command *cmd = (Command *) node->data; 
    
            if(fd_in != 0){
                dup2(fd_in, 0); 
                close(fd_in); 
            }

            if(node->next != NULL){
                close(pipe_fds[0]); 
                dup2(pipe_fds[1], 1); 
                close(pipe_fds[1]); 
            }

            if(handleCommandRedirects(cmd->redirections)) _exit(1);   

            execvp(cmd->args[0], cmd->args); 
            _exit(1); 
        }  
        else if(pid < 0){
            return 1;  
        } 
        else {
            if(fd_in != 0) close(fd_in);

            if(node->next != NULL) {
                close(pipe_fds[1]); 
                fd_in = pipe_fds[0];
            } 
            numCommands++;
            node = node->next;
        }
    }

    for(int i = 0; i < numCommands; i++){
        int status; 
        pid_t pidWait = wait(&status); 
        if(pidWait == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0){
            return 1; 
        }
    }

    return 0; 
}