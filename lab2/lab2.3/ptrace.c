#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    pid_t proc = fork();
    if(proc == -1){
        perror("fork");
        return 1;
    }
    if(proc == 0){
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], argv + 1);
        printf("error in execvp");
        return 1;
    } else {
        int status;
        struct user_regs_struct regs;

        waitpid(proc, &status, 0);
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
            ptrace(PTRACE_GETREGS, proc, NULL, &regs);

            long syscall_number = regs.orig_rax;
            printf("Syscall number: %ld\n", syscall_number);

            ptrace(PTRACE_SYSCALL, proc, NULL, NULL);
            waitpid(proc, &status, 0);
        }
    }
}