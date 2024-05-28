#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>

#define FILE_PATH "./file.txt"

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
    
    char message[1024];
    while(1){
        printf("enter your message:");
        fgets(message,1024, stdin);
        int send_len = send(sock_fd, message, 1024, 0);
        if(send_len <= 0){
            perror("error in sending");
            break;
        }
        int recv_len = recv(sock_fd, message, 1024, 0);
        if(recv_len <= 0){
            perror("error in receiving");
            break;
        }
        message[recv_len-1] = '\0';
        printf("message from server: %s", message);
    }
    close(sock_fd);
    return 0;
}