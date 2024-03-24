#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int hello_write(int fd, char* message, int size){
    return syscall(SYS_write, fd, message, size);
}

int main(){
    char* mes = "Hello world";
    int res = hello_write(STDOUT_FILENO, mes, strlen(mes));
    if(res == -1){
        perror("write");
        return 1;
    }
    return 0;
}