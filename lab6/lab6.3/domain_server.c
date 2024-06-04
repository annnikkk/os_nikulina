#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define FILE_PATH "./file.txt"
#define MESSAGE_LEN 1024


int main(){
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock_fd == -1){
        perror("error in socket");
        return 1;
    }
    
    struct sockaddr_un server_addr = {0};
    struct sockaddr_un client_addr = {0};
    server_addr.sun_family = AF_UNIX;
    char* file_path = FILE_PATH;
    strncpy(server_addr.sun_path, file_path, sizeof(server_addr.sun_path) - 1);
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
    
    socklen_t client_addr_len = sizeof(client_addr);
    char message[1024];
    
    while(1){
        int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd == -1){
            perror("error in accepting client");
            close(sock_fd);
            continue;
        }
        pid_t new_pid = fork();
        if(new_pid == -1){
            perror("error in fork");
            close(client_fd);
            continue;
        }
        if(new_pid == 0){
            close(sock_fd);
            
            /*if(getpeername(client_fd, (struct sockaddr*) &client_addr, &client_addr_len) == -1){
                perror("error in getpeername");
                close(client_fd);
                break;
            }*/
            
            while(1){
                ssize_t len = recv(client_fd, message, MESSAGE_LEN - 1, 0);
                if(len == -1){
                    perror("error in recv");
                    break;
                }
                message[len] = '\0';
                printf("client message: %s", message);
                if(send(client_fd, message, len, 0) == -1){
                    perror("error in send");
                    break;
                }
            }
        }
        close(client_fd);
    }
    close(sock_fd);
    return 0;
}