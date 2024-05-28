#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>


void recursion(int i) {
    if (i >= 10) return;
    char str[] = "hello world";
    recursion(i + 1);
}

int doing_recursion() {
    recursion(0);
    return 15;
}

int main() {
    int fd = open("res_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("error in opening file");
        return 1;
    }
    
    if (ftruncate(fd, sysconf(_SC_PAGESIZE)) == -1) { //чтобы файл был размера страницы
        perror("error in ftruncate");
        close(fd);
        return 1;
    }
    
    void *child_stack = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (child_stack == MAP_FAILED) {
        perror("error in mmap");
        close(fd);
        return 1;
    }
    
    pid_t pid = clone(doing_recursion, child_stack + sysconf(_SC_PAGESIZE), SIGCHLD, NULL);
    if (pid == -1) {
        perror("error in clone");
        munmap(child_stack, sysconf(_SC_PAGESIZE));
        close(fd);
        return 1;
    }
    
    int status;
    waitpid(pid, &status, 0);
    
    munmap(child_stack, sysconf(_SC_PAGESIZE));
    close(fd);
    return 0;
}

