#include "shared.h"

ssize_t getFileSize(const char* file_name){
    FILE* file_ptr = fopen(file_name, "rb");

    if(file_ptr == nullptr) return -1;
    
    fseek(file_ptr, 0, SEEK_END);
    ssize_t size = ftell(file_ptr);
    fclose(file_ptr);
    return size;
}

int main(){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8080);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_fd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) != -1){
        std::cout << "Successfully connected to server!\n";

        // send header packet

        char* test_file = "source/plaintext.txt";
        struct file_header file_metadata;
        strcpy(file_metadata.file_name, test_file);
        file_metadata.file_size = getFileSize(test_file); // for now
        send(client_fd, (struct file_header*) &file_metadata, sizeof(file_metadata), 0);
    }

    close(client_fd);
    return 0;
}