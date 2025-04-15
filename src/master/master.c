#include "master.h"
// #include "../metadata/metadata.h"
// #include "../replication/replication.h"
#include "../network/network.h"
#include "metadata.h"
#include "replication.h"

void *client_handler(void *arg) {
  int client_socket = *(int *)arg; // client handler is a pthread so it might
                                   // dynamically allocate the memory
  free(arg);

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
  if (bytes_received <= 0) {
    perror("[MASTER] Failed to receive data from client");
    close(client_socket);
    pthread_exit(NULL);
  }

  printf("[MASTER] Received request: %s\n", buffer);
  handle_client_request(client_socket, buffer);

  close(client_socket);
  pthread_exit(NULL);
}

void handle_client_request(int client_socket, char *request) {
  // Dummy parsing: expecting "UPLOAD <filename>"
  char command[16], filename[256];
  sscanf(request, "%s %s", command, filename);

  if (strcmp(command, "UPLOAD") == 0) {
    // Simulate storing metadata
    update_metadata(filename, "Node1:/storage/node1/");

    // Simulate replication
    replicate_file(filename, "127.0.0.1",
                   9001); // Replace with actual node IP/port

    char response[] = "File uploaded and replicated successfully.\n";
    send(client_socket, response, strlen(response), 0);
  } else if (strcmp(command, "GET") == 0) {
    char response[BUFFER_SIZE];
    retrieve_metadata(filename, response);
    send(client_socket, response, strlen(response), 0);
  } else {
    char response[] = "Invalid command.\n";
    send(client_socket, response, strlen(response), 0);
  }
}

void start_master_node() {
  int server_socket = initialize_server(MASTER_PORT);
  if (server_socket < 0) {
    perror("[MASTER] Server initialization failed");
    exit(EXIT_FAILURE);
  }

  printf("[MASTER] Master node listening on port %d...\n", MASTER_PORT);

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int *client_socket = malloc(sizeof(int));

    *client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (*client_socket < 0) {
      perror("[MASTER] Failed to accept client");
      free(client_socket);
      continue;
    }

    printf("[MASTER] Connection accepted from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    pthread_t tid;
    pthread_create(&tid, NULL, client_handler, client_socket);
    pthread_detach(tid);
  }

  close(server_socket);
}
