//define all the functions and constants used in the master node 
#ifndef MASTER_H
#define MASTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MASTER_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

void start_master_node();
void handle_client_request(int client_socket, char *request);

#endif // MASTER_H
