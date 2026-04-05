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
        char* source = "source/plaintext.txt";
        char* destination = "destination/output.txt";

        struct file_header file_metadata;
        strcpy(file_metadata.source, source);
        strcpy(file_metadata.destination, destination);
        file_metadata.file_size = getFileSize(source);

        send(client_fd, (struct file_header*) &file_metadata, sizeof(file_metadata), 0);
        
        // now send all the bytes over
        FILE* file_ptr = fopen(file_metadata.source, "rb");
        ssize_t total_bytes = 0;
        while(total_bytes < file_metadata.file_size){
            // set to 1 for testing
            char buffer[CHUNK_SIZE];
            ssize_t bytes_read = fread(buffer, sizeof(char), CHUNK_SIZE, file_ptr);
            total_bytes += bytes_read;
            if(send(client_fd, &buffer[0], bytes_read, 0) <= 0){
                break;
            }
            std::cout << "Progress: " << total_bytes << "/" << file_metadata.file_size << " bytes..." << std::endl;
        }
    }

    close(client_fd);
    return 0;
}