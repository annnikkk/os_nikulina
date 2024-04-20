#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    pid_t pid = getpid();
    printf("current pid: %d\n", pid);
    sleep(1);
    execv(argv[0], argv);
    printf("Hello world\n");
    return 0;
}