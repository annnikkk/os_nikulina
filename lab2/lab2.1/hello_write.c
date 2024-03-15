#include <unistd.h>

int main(){
    char* message = "Hello world";
    int res = write(STDOUT_FILENO, message, strlen(message));
    if(res == -1 && errno != EINTR){
        perror("write");
        return 1;
    }
    return 0;
}