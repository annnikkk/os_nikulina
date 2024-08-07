#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#define PORT 1235
#define MESSAGE_LEN 1024


int main(){
    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
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
    
    char message[MESSAGE_LEN];
    while(1){
        printf("enter your message:");
        fgets(message,MESSAGE_LEN, stdin);
        ssize_t send_len = send(sock_fd, message, MESSAGE_LEN, 0);
        if(send_len <= 0){
            perror("error in sending");
            close(sock_fd);
            break;
        }
        ssize_t recv_len = recv(sock_fd, message, MESSAGE_LEN, 0);
        if(recv_len <= 0){
            perror("error in receiving");
            close(sock_fd);
            break;
        }
        message[recv_len-1] = '\0';
        printf("message from server: %s", message);
    }
    
    close(sock_fd);
    return 0;
}