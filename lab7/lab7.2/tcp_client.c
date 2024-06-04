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
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);
    
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("error in connecting");
        return 1;
    }
    
    char message[1024];
    while(1){
        printf("enter your message:");
        fgets(message,1024, stdin);
        int send_len = send(sock_fd, message, sizeof(message), 0);
        if(send_len <= 0){
            perror("error in sending");
            close(sock_fd);
            break;
        }
        ssize_t recv_len = recv(sock_fd, message, sizeof(message), 0);
        if(recv_len <= 0){
            if(len == 0) printf("host %d disconnected", client_fd);
            else perror("error in receiving");
            break;
        }
        printf("message from server: %s", message);
    }
    close(sock_fd);
    return 0;
}