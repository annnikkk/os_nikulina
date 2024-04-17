#include <stdio.h>

int* CreatingLocalVar(){
    int local_var = 1786416521;
    printf("local var address in function: %p\n", &local_var);
    printf("local var value in function: %d\n", local_var);
    int* p = &local_var;
    return p;
}

int main(){
    int* local_var_address = CreatingLocalVar();
    printf("\n");
    printf("local var address in main: %p\n", local_var_address);
    printf("local var value in main: %d\n", *local_var_address);
    return 0;
}