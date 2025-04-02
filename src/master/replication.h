#ifndef REPLICATION_H
#define REPLICATION_H

#define REPLICATION_FACTOR 2  //number of replicas per file 

//fucntion to replicate field to multiple datanodes
void replicate_file(const char *filename, const char *source_node, const char **replica_nodes, int num_nodes);

// Function to send a file from one node to another
int send_file_to_node(const char *filename, const char *source_node, const char *target_node);

#endif // REPLICATION_H
