#include <stdio.h>
#include <stdlib.h>

#ifndef MAP_H
#define MAP_H

#define MAX_NODES 10000
#define MAX_LINE_LENGTH 10000

/* Define data structures */
typedef struct edge_t {
    int id;
    int node1;
    int node2;
    double length;
    struct edge_t* next;
} edge_t;

typedef struct node_t {
    int id;
    double lat;
    double lon;
    edge_t* edges;
    int num_edges;
    struct node_t* next;
} node_t;

typedef struct map_t {
    node_t* nodes;
    int num_nodes;
    int num_edges;
} map_t;

#endif