#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "map.h"

int hash(int key, int size) {
    return key % size;
}

hashtable_t* hashtable_create(int size) {
    hashtable_t* hashtable = malloc(sizeof(hashtable_t));
    hashtable->size = size;
    hashtable->table = malloc(sizeof(node_t*) * size);
    memset(hashtable->table, 0, sizeof(node_t*) * size);
    return hashtable;
}

void hashtable_put(hashtable_t* hashtable, int key, node_t* value) {
    int index = hash(key, hashtable->size);
    node_t* new_node = malloc(sizeof(node_t));
    memcpy(new_node, value, sizeof(node_t));
    while (hashtable->table[index] != NULL && hashtable->table[index]->id != key) {
        index = (index + 1) % hashtable->size;
    }
    hashtable->table[index] = new_node;
}


node_t* hashtable_get(hashtable_t* hashtable, int key) {
    int index = hash(key, hashtable->size);
    while (hashtable->table[index] != NULL && hashtable->table[index]->id != key) {
        index = (index + 1) % hashtable->size;
    }
    if (hashtable->table[index] != NULL) {
        return hashtable->table[index];
    } else {
        return NULL;
    }
}

void dijkstra(map_t* map, int start_id, int end_id) {
    double dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int prev[MAX_NODES];
    int i, u, v;
    node_t* nodes = map->nodes;
    edge_t* e;
    hashtable_t* hashtable = hashtable_create(map->num_nodes);

    for (node_t* n = nodes; n != NULL; n = n->next) {
        hashtable_put(hashtable, n->id, n);
    }

    for (i = 0; i < map->num_nodes; ++i) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start_id] = 0;

    for (i = 0; i < map->num_nodes; ++i) {
        u = -1;
        for (node_t* n = nodes; n != NULL; n = n->next) {
            if (!visited[n->id] && (u == -1 || dist[n->id] < dist[u]))
                u = n->id;
        }
        if (dist[u] == INT_MAX)
            break;
        visited[u] = 1;

        node_t* u_node = hashtable_get(hashtable, u);
        e = u_node->edges;
        while (e != NULL) {
            node_t* v_node = hashtable_get(hashtable, e->node2);
            v = v_node->id;
            if (!visited[v]) {
                double new_dist = dist[u] + e->length;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    prev[v] = u;
                }
            }
            e = e->next;
        }
    }

    if (prev[end_id] == -1) {
        printf("No path found\n");
    } else {
        int path[MAX_NODES];
        int path_len = 0;
        for (u = end_id; u != -1; u = prev[u])
            path[path_len++] = u;
        printf("Shortest path: ");
        for (i = path_len - 1; i >= 0; --i) {
            printf("%d", path[i]);
            if (i > 0)
                printf(" -> ");
        }
        printf("\n");
        printf("Distance: %f\n", dist[end_id]);
    }

    for (int i = 0; i < hashtable->size; i++) {
        if (hashtable->table[i] != NULL) {
            free(hashtable->table[i]);
        }
    }

    free(hashtable->table);
    free(hashtable);
}