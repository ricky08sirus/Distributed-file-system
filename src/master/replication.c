#include "replication.h"
#include "../network/network.h"
#include <string.h>

const char *available_nodes[] = {
    "127.0.0.1",  // Node 1
    "127.0.0.2",  // Node 2
    "127.0.0.3"   // Node 3
};

const int available_ports[] = {9001, 9002, 9003};
const int total_nodes = 3;

ReplicationInfo get_replication_targets(){
  ReplicationInfo info;
  for (int i = 0; i < REPLICATION_COUNT; i++) {
        strncpy(info.node_ips[i], available_nodes[i % total_nodes], sizeof(info.node_ips[i]));
        info.node_ports[i] = available_ports[i % total_nodes];
    }
    return info;

}

void replicate_file(const char *filename, const char *source_node_ip, int source_node_port) {
    ReplicationInfo targets = get_replication_targets();

    printf("[REPLICATION] Starting replication for file: %s\n", filename);

    for (int i = 0; i < REPLICATION_COUNT; i++) {
        printf("[REPLICATION] Replicating to %s:%d\n", targets.node_ips[i], targets.node_ports[i]);
        int result = send_file(filename, targets.node_ips[i], targets.node_ports[i]);
        if (result == 0) {
            printf("[REPLICATION] Successfully replicated to %s:%d\n", targets.node_ips[i], targets.node_ports[i]);
        } else {
            printf("[REPLICATION] Failed to replicate to %s:%d\n", targets.node_ips[i], targets.node_ports[i]);
        }
    }
}


