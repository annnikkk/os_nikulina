int main() {
    char* message= "Hello world";
    asm (
        "movq $1, %%rax\n"
        "movq $1, %%rdi\n"
        "movq %0, %%rsi\n"
        "movq $11, %%rdx\n"
        "syscall\n"
        :
        : "r"(message)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    return 0;
}

