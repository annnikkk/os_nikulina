#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int global_var = 50;

int main(){
    int local_var = 60;
    printf("Global var address: %p\n", &global_var);
    printf("Global var content: %d\n", global_var);
    
    printf("Local var address: %p\n", &local_var);
    printf("Local var content: %d\n", local_var);
    
    printf("current pid: %d\n", getpid());
    
    pid_t child_pid;
    child_pid = fork();
    
    if (child_pid == -1) {
        perror("error in fork");
        return 1;
    }
    if(child_pid == 0){
        printf("Child pid: %d\n", getpid());
        printf("Parent pid: %d\n", getppid());
        
        printf("Global var address in child proc: %p\n", &global_var);
        printf("Global var content in child proc: %d\n", global_var);
        
        printf("Local var address in child proc: %p\n", &local_var);
        printf("Local var content in child proc: %d\n", local_var);
        
        global_var = 150;
        local_var = 160;
        
        printf("Changed global var content in child proc: %d\n", global_var);
        printf("Changed local var content in child proc: %d\n", local_var);
        
        sleep(20);
        exit(5);
        //тут показан процесс усыновления дочернего процесса, отследить это можно с помощью top
        
    } else {
        printf("Global var content in parent proc: %d\n", global_var);
        printf("Local var content in parent proc: %d\n", local_var);
    }
    return 0;
}