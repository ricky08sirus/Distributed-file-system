#ifndef  REPLICATION_H
#define REPLICATION_H

#include <stdio.h>

//constants
//
#define REPLICATION_COUNT 2
#define MAX_NODES 10 

//structure to hold the replication info 
//
typedef struct {
  char node_ips[REPLICATION_COUNT][50];
  int node_ports[REPLICATION_COUNT];
} ReplicationInfo;



// Function Declarations
void replicate_file(const char *filename, const char *source_node_ip, int source_node_port);
ReplicationInfo get_replication_targets();

#endif // REPLICATION_H



