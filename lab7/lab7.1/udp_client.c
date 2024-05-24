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
        fgets(message,1024, stdin);
        int send_len = sendto(sock_fd, message, 1024, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        printf("i'm here %d", send_len);
        int recv_len = recvfrom(sock_fd, message, 1024, 0, (struct sockaddr*)&server_addr, &server_addr_len);
        message[recv_len-1] = '\0';
        printf("message from server: %s", message);
    }
    return 0;
}