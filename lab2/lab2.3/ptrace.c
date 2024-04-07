#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("wrong arguments\n use: <command_name> ");
        return 1;
    }
    pid_t proc = fork();
    if(proc == -1){
        perror("fork");
        return 1;
    }
    if(proc == 0){
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1){
            perror("error in PTRACE_TRACEME");
            return 1;
        }
        execvp(argv[1], argv + 1);
        printf("error in execvp");
        return 1;
    } else {
        int status;
        struct user_regs_struct regs;
        
        waitpid(proc, &status, 0);
        while(!WIFEXITED(status) && !WIFSIGNALED(status)){
            if(ptrace(PTRACE_GETREGS, proc, NULL, &regs) == -1){
                perror("error in PTRACE_GETREGS");
            }
            
            long syscall_number = regs.orig_rax;
            printf("Syscall number: %ld\n", syscall_number);
            
            if(ptrace(PTRACE_SYSCALL, proc, NULL, NULL) == -1){
                perror("error in PTRACE_SYSCALL");
                return 1;
            }
            if(waitpid(proc, &status, 0) == -1){
                perror("error in waitpid");
                return 1;
            }
        }
    }
    return 0;
}