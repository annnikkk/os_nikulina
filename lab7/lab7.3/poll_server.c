#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#define PORT 1235


int main(){
    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
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
    int server_addr_len = sizeof(server_addr);
    
    
    if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
        perror("error in bind");
        return 1;
    }
    
    if(listen(sock_fd, 5) == -1){
        perror("error in listen");
        return 1;
    }
    
    int client_sockets[5], active;
    struct pollfd fd[6];
    
    memset(client_sockets, 0, sizeof(client_sockets));
    memset(fd, 0, sizeof(fd));
    
    fd[0].fd = sock_fd;
    fd[0].events = POLLIN;
    
    char message[1024];
    int client_fd;
    while(1){
        active = poll(fd, 6, -1);
        if((active < 0) && (errno != EINTR)) {
            perror("error in poll");
            break;
        }
        if(fd[0].revents & POLLIN){
            fd[0].revents = 0;
            if((client_fd = accept(sock_fd, (struct sockaddr *)&server_addr, (socklen_t *)&server_addr_len)) == -1) {
                perror("error in accept");
                break;
            }
            
            for(int i = 1; i < 6; i++) {
                if(client_sockets[i-1] == 0) {
                    client_sockets[i-1] = client_fd;
                    fd[i].fd = client_fd;
                    fd[i].revents = POLLIN;
                    break;
                } else{
                    //printf("%d-ое место занято, там сидит бабайка %d\n", i-1, client_sockets[i-1]);
                }
            }
        }
        for(int i = 1; i < 6; i++) {
            if(client_sockets[i-1] > 0 && (fd[i].revents & POLLIN)) {
                fd[i].revents = 0;
                ssize_t read_bytes;
                if((read_bytes = recv(client_sockets[i-1], message, 1024, 0)) <= 0) {
                    if(read_bytes == 0) printf("host %d disconnected\n", client_sockets[i-1]);
                    else perror("error in recv");
                    close(client_sockets[i-1]);
                    client_sockets[i-1] = 0;
                } else {
                    message[read_bytes] = '\0';
                    printf("client %d says: %s", i-1, message);
                    send(client_sockets[i-1], message, read_bytes, 0);
                    fd[i].events |= POLLIN;
                }
            }
        }
    }
    close(sock_fd);
    return 0;
}