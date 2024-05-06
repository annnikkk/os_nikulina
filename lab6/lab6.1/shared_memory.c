#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>




int main(){
    long page_size = sysconf(_SC_PAGE_SIZE);
    
    if (page_size == -1) {
        perror("error in sysconf");
        return 1;
    }
    unsigned int *shared_memory = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("error mmap");
        exit(EXIT_FAILURE);
    }
    pid_t reading_pid = fork();
    if(reading_pid == -1){
        perror("error in fork");
        return 1;
    }
    unsigned int max_value = UINT_MAX;
    unsigned int previous_var = -1;
    unsigned int cur_value = 0;
    unsigned int* reader_pointer = shared_memory;
    unsigned int* writing_pointer = shared_memory;
    if(reading_pid == 0){
        while(1){
            cur_value = *reader_pointer;
            if(cur_value != previous_var + 1){
                printf("sequence failure: previous var = %u, current var = %u\n", previous_var, cur_value);
            } else {
                printf("all right, expected %u, cur val = %u\n", previous_var + 1, cur_value);
            }
            previous_var = cur_value;
            reader_pointer++;
            if(reader_pointer == shared_memory + page_size / sizeof(unsigned int)) reader_pointer = shared_memory;
            sleep(1);
        }
    } else {
        while(1){
            *writing_pointer = cur_value;
            cur_value++;
            if(cur_value == max_value) cur_value = 0;
            writing_pointer++;
            if(writing_pointer == shared_memory + page_size / sizeof(unsigned int)) writing_pointer = shared_memory;
            sleep(1);
        }
        waitpid(reading_pid, NULL, 0);
    }
    return 0;
}