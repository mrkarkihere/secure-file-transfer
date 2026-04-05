#include "shared.h"
#include <thread>

//
void handle_client(int fd, struct sockaddr_in* addr, socklen_t addrlen){
    struct file_header file_metadata;
    ssize_t header_bytes = recv(fd, &file_metadata, sizeof(file_metadata), 0);

    if(header_bytes > 0){
        std::cout << "Source: " << file_metadata.source << std::endl;
        std::cout << "Destination: " << file_metadata.destination << std::endl;
        std::cout << "File Size: " << file_metadata.file_size << " bytes" << std::endl;
        
        // just create the output file, dont write to it yet
        FILE* file_ptr = fopen(file_metadata.destination, "wb");
        ssize_t total_bytes = 0;
        // receive bytes
        while(total_bytes < file_metadata.file_size){
            char buffer[CHUNK_SIZE];
            ssize_t bytes_recv = recv(fd, &buffer[0], sizeof(buffer), 0);
            // connection ended or receive failed
            if(bytes_recv <= 0){
                break;
            }
            total_bytes += bytes_recv;
            fwrite(buffer, sizeof(char), bytes_recv, file_ptr);
        }
        if(total_bytes == file_metadata.file_size){
            std::cout << "Received all bytes correctly" << std::endl;
        }
        fclose(file_ptr);

    }
    else{
        std::cout << "Didn't receive bytes correctly" << std::endl;
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
    serveraddr.sin_port = htons(8080); 
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

    while(true){
        // client info
        struct sockaddr_in clientaddr;
        socklen_t addrlen = sizeof(clientaddr);
        int client_fd = accept(server_fd, (struct sockaddr*) &clientaddr , &addrlen); // blocking call?

        if(client_fd != -1){
            std::cout << "Connection established with FD " << std::to_string(client_fd) << "\n";
            std::thread t(handle_client, client_fd, &clientaddr, addrlen);
            t.detach();
        }
    }

    std::cout << "Closing server socket\n";

    close(server_fd);
    return 0;
}