#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234


int main(){
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1){
        perror("error in socket");
        return 1;
    }
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
        perror("error in bind");
        close(sock_fd);
        return 1;
    }
    socklen_t client_addr_len = sizeof(client_addr);
    char message[1024];
    while(1){
        ssize_t len = recvfrom(sock_fd, message, sizeof(message)-1, 0, (struct sockaddr*) &client_addr, &client_addr_len);
        if(len  == -1){
            perror("error in recvfrom");
            break;
        }
        message[len] = '\0';
        printf("client message: %s", message);
        if(sendto(sock_fd, message, len, 0, (struct sockaddr*)&client_addr, client_addr_len) == -1){
            perror("error in sendto");
            break;
        }
    }
    close(sock_fd);
    return 0;
}