#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234


int main(){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        perror("error in socket");
        return 1;
    }
    int opt = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("error in setsockopt");
        close(sock_fd);
        return 1;
    }
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
        perror("error in bind");
        return 1;
    }
    
    if(listen(sock_fd, 5) == -1){
        perror("error in listen");
        return 1;
    }
    socklen_t client_addr_len = sizeof(client_addr);
    char message[1024];
    while(1){
        int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd == -1){
            perror("error in accepting client");
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
            while(1){
                ssize_t len = recv(client_fd, message, sizeof(message)-1, 0);
                if(len <= 0){
                    if(len == 0) printf("host %d disconnected", client_fd);
                    else perror("error in recv");
                    break;
                }
                message[len] = '\0'; //так и было написано, но сейчас я принимаю 1023 и последним ставлю символ конца строки
                printf("client message: %s", message);
                if(send(client_fd, message, len, 0) == -1){
                    perror("error in send");
                    break;
                }
            }
            close(client_fd);
            return 0;
        }
        close(client_fd);
    }
    close(sock_fd);
    return 0;
}