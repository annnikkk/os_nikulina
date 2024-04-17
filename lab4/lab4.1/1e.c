#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void BufferWork(){
    char* first_buf = (char*) malloc(sizeof(char) * 100);
    strcpy(first_buf, "HELLO WORLD");
    printf("Content of first buf: %s\n", first_buf);
    free(first_buf);
    printf("Content of first buf after free: %s\n", first_buf);
    
    char* second_buf = (char*) malloc(sizeof(char) * 100);
    strcpy(second_buf, "hello world");
    printf("Content of second buf: %s\n", second_buf);
    char* middle = second_buf + strlen(second_buf) / 2;
    free(middle);
    printf("Content of middle of second buf: %s\n", second_buf);
    free(second_buf);
}

int main(){
    BufferWork();
}