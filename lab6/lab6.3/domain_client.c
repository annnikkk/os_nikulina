#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    server_addr.sun_family = AF_UNIX;
    
    char* file_path = FILE_PATH;
    strncpy(server_addr.sun_path, file_path, sizeof(server_addr.sun_path) - 1);
    
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("error in connecting");
        close(sock_fd);
        return 1;
    }
    
    char message[MESSAGE_LEN];
    while(1){
        printf("enter your message:");
        fgets(message, MESSAGE_LEN, stdin);
        ssize_t send_len = send(sock_fd, message, MESSAGE_LEN, 0);
        if(send_len <= 0){
            perror("error in sending");
            break;
        }
        ssize_t recv_len = recv(sock_fd, message, MESSAGE_LEN, 0);
        if(recv_len <= 0){
            perror("error in receiving");
            break;
        }
        printf("message from server: %s", message);
    }
    close(sock_fd);
    return 0;
}