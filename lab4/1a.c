#include <stdio.h>
#include <unistd.h>

int global_not_initiated;
int global_initiated = 5;
const int global_const = 2;

void foo(){
    int local_in_foo = 5;
    static int static_in_foo = 4;
    const int const_in_foo = 3;
    
    printf("local in foo: %p\n", &local_in_foo);
    printf("static in foo: %p\n", &static_in_foo);
    printf("const in foo: %p\n", &const_in_foo);
}

int main(){
    printf("global not initiated: %p\n", &global_not_initiated);
    printf("global initiated: %p\n", &global_initiated);
    printf("global const: %p\n", &global_const);
    foo();
    
    printf("process id: %d\n", getpid());
    getchar();
    return 0;
}