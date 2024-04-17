#include <stdio.h>
#include <stdlib.h>

int main(){
    char* env_var = getenv("MY_NEW_ENV");
    if(env_var != NULL){
        printf("environment var value: %s\n", env_var);
    } else {
        perror("error in getting value of env var");
        return 1;
    }
    setenv("MY_NEW_ENV", "777", 1);
    char* new_env_var = getenv("MY_NEW_ENV");
    if(new_env_var != NULL){
        printf("environment var new value: %s\n", new_env_var);
    } else {
        perror("error in changing value of env var");
        return 1;
    }
    return 0;
}