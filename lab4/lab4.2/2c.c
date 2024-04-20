#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>


int I = 0;

void RecursionStack(){
    if(I == 10) return;
    char arr[4096*10];
    I++;
    sleep(1);
    RecursionStack();
}

void CycleHeap(){
    char* buffers[10];
    for(int i = 0; i < 10; i++){
        buffers[i] = (char*) malloc(sizeof(char) * 4096* 50);
        sleep(1);
    }
    for(int i = 0; i < 10; i++){
        free(buffers[i]);
    }
}

void sigsegv_handler(int signum){
    printf("Segmentation fault occurred.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    pid_t pid = getpid();
    printf("current pid: %d\n", pid);
    sleep(10);
    RecursionStack();
    CycleHeap();
    signal(SIGSEGV, sigsegv_handler);
    void* region = mmap(NULL, getpagesize() * 10, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if(region == MAP_FAILED){
        perror("mmap failed");
        return 1;
    }
    printf("mapped region successfully\n");
    
    /*if(mprotect(region, getpagesize() * 10, PROT_NONE) == -1){
        perror("mprotect failed");
        return 1;
    }*/
    //printf("protected region successfully\n");
    char trying_reading = *((char*) region);
    if(trying_reading != -1){
        printf("I can read.");
    } else {
        perror("error reading");
    }
    /*char* trying_writing = "It's my try.";
    strcpy((char*) region, trying_writing);
    printf("Region after try: %s\n", (char*) region);*/
    if(munmap(region + getpagesize() * 4, getpagesize() * 3) == -1){
        perror("error in munmap");
        return 1;
    }
    return 0;
}