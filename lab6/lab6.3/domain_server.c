#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

#define FILE_PATH "./file.txt"
#define MESSAGE_LEN 1024

int sock_fd = -1;
volatile sig_atomic_t server_running = 1;

void handle_sigint(int sig) {
    (void)sig;
    server_running = 0;
    if (sock_fd != -1) {
        close(sock_fd);
        sock_fd = -1;
    }
}

void handle_child_sigint(int sig) {
    (void)sig;
    exit(0);
}

void handle_sigchld(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(){
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock_fd == -1){
        perror("error in socket");
        return 1;
    }
    
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, FILE_PATH, sizeof(server_addr.sun_path) - 1);
    unlink(FILE_PATH);
    
    if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
        perror("error in bind");
        close(sock_fd);
        return 1;
    }
    
    if(listen(sock_fd, 5) == -1){
        perror("error in listen");
        close(sock_fd);
        return 1;
    }
    
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, handle_sigchld);
    
    socklen_t client_addr_len = sizeof(client_addr);
    char message[MESSAGE_LEN];
    
    while(server_running){
        int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd == -1){
            if(errno == EINTR) {
                continue;
            }
            perror("error in accepting client");
            close(sock_fd);
            return 1;
        }
        
        pid_t new_pid = fork();
        if(new_pid == -1){
            perror("error in fork");
            close(client_fd);
            continue;
        }
        
        if(new_pid == 0){
            close(sock_fd);
            signal(SIGINT, handle_child_sigint);
            
            while(1){
                ssize_t len = recv(client_fd, message, MESSAGE_LEN, 0);
                if(len == -1){
                    perror("error in recv");
                    break;
                } else if (len == 0) {
                    break;
                }
                
                message[len] = '\0';
                printf("client message: %s", message);
                
                if(send(client_fd, message, len, 0) == -1){
                    perror("error in send");
                    break;
                }
            }
            close(client_fd);
            return 0;
        } else {
            close(client_fd);
        }
    }
    
    while (waitpid(-1, NULL, 0) > 0);
    
    close(sock_fd);
    unlink(FILE_PATH);
    return 0;
}