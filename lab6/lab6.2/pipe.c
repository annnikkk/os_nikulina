#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

int main(){
    int pipe_fds[2];
    if(pipe(pipe_fds) == -1){
        perror("error in pipe");
        return 1;
    }
    pid_t reading_pid = fork();
    if(reading_pid == -1){
        perror("error in fork");
        return 1;
    }
    unsigned int previous_var = -1;
    unsigned int cur_value = 0;
    unsigned int max_value = UINT_MAX;
    
    if(reading_pid == 0){
        while(1){
            read(pipe_fds[0], &cur_value, sizeof(unsigned int));
            if(cur_value != previous_var + 1){
                printf("sequence failure: previous var = %u, current var = %u\n", previous_var, cur_value);
            } else {
                printf("all right, expected %u, cur val = %u\n", previous_var + 1, cur_value);
            }
            previous_var = cur_value;
            sleep(1);
        }
    } else {
        while(1){
            write(pipe_fds[1], &cur_value, sizeof(unsigned int));
            cur_value++;
            if(cur_value == max_value) cur_value = 0;
            sleep(1);
        }
    }
    return 0;
}