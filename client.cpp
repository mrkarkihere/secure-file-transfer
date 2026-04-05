#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>


int main(){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8081);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_fd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) != -1){
        std::cout << "Successfully connected to server!\n";

        const char* message = "this is the msg sent";
        send(client_fd, message, strlen(message), 0);
    }

    close(client_fd);
    return 0;
}