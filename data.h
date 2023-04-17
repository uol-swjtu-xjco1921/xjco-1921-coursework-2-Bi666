#include <stdio.h>
#include <stdlib.h>

#ifndef DATA_H
#define DATA_H

#define MAX_LOCATIONS 1000

// Graph represented as an adjacency list
typedef struct {
    int id;
    double lat;
    double lon;
    // other properties can be added here
} node_t;

typedef struct {
    int id;
    int node1;
    int node2;
    int way;
    double length;
    double veg;
    double arch;
    double land;
    int POI[];
    // other properties can be added here
} link_t;

typedef struct {
    node_t* nodes;
    int num_nodes;
    link_t* links;
    int num_links;
} adjacency_list_t;

#endif