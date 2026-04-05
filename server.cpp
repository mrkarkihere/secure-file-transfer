#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

//
void handle_client(int fd, struct sockaddr_in* addr, socklen_t addrlen){
    char buffer[1024];
    ssize_t bytes = recv(fd, &buffer[0], sizeof(buffer), 0);
    
    if(bytes > 0){
        std::cout << buffer << std::endl;
    }
    close(fd);
}
// 
int main(){

    // create the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // ipv4, TCP, default (tcp)

    // server address
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET; // tcp
    serveraddr.sin_port = htons(8081); 
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // assign an address to listen in
    if(bind(server_fd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) == -1){
        std::cout << "Binding failed!\n";
        return 0;
    }

    // which socket to listen to, queue length on backlog
    if(listen(server_fd, 3) == -1){
        std::cout << "Listen call failed!\n";
        return 0;
    }

    // client info
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    int client_fd = accept(server_fd, (struct sockaddr*) &clientaddr , &addrlen); // blocking call?

    if(client_fd != -1){
        std::cout << "Connection established with FD " << std::to_string(client_fd) << "\n";
        std::thread t(handle_client, client_fd, &clientaddr, addrlen);
        t.detach();
    }

    std::cout << "Closing server socket\n";

    close(server_fd);
    return 0;
}