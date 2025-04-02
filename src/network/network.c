#include "network.h"
int send_file(const char *filename, const char *destination_ip, int destination_port) {
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *file;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(destination_port);
    server_addr.sin_addr.s_addr = inet_addr(destination_ip);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sockfd);
        return -1;
    }

    // Open the file
    file = fopen(filename, "rb");
    if (!file) {
        perror("File open failed");
        close(sockfd);
        return -1;
    }

     // Send the file data
     size_t bytes_read;
     while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
         send(sockfd, buffer, bytes_read, 0);
     }

     printf("File %s sent successfully.\n", filename);
     fclose(file);
     close(sockfd);
     return 0;
}

int receive_file(const char *filename, int listen_port){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];


     // Create socket
     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(listen_port);

     // Bind the socket
     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }
        // Listen for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    printf("Waiting for a file on port %d...\n", listen_port);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        return -1;
    }
     // Open the file to write received data
     FILE *file = fopen(filename, "wb");
     if (!file) {
        perror("File open failed");
        close(new_socket);
        close(server_fd);
        return -1;
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }
    printf("File %s received successfully.\n", filename);
    fclose(file);
    close(new_socket);
    close(server_fd);
    return 0;

    


}