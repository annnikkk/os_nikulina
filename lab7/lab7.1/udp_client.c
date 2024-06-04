#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 1234


int main(){
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1){
        perror("error in socket");
        return 1;
    }
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);
    
    char message[1024];
    socklen_t server_addr_len = sizeof(server_addr);
    while(1){
        printf("enter your message:");
        fgets(message, sizeof(message), stdin);
        int send_len = sendto(sock_fd, message, sizeof(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        printf("i'm here %d", send_len);
        ssize_t recv_len = recvfrom(sock_fd, message, sizeof(message), 0, (struct sockaddr*)&server_addr, &server_addr_len);
        printf("message from server: %s", message);
    }
    close(sock_fd);
    return 0;
}