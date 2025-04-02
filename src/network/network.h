#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 9000
#define MAX_CONNECTIONS 5

// Function to send a file to another node
int send_file(const char *filename, const char *destination_ip, int destination_port);

// Function to receive a file
int receive_file(const char *filename, int listen_port);

// Function to initialize a network connection
int initialize_server(int port);

// Function to establish a client connection
int connect_to_server(const char *ip, int port);

#endif // NETWORK_H
