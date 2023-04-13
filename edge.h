#include <stdio.h>
#include <stdlib.h>

#ifndef DIST_H
#define DIST_H

#define MAX_NODES 1000
#define MAX_LOCATIONS 1000

typedef struct edge {
    int node_id;
    double distance;
    double speed_limit;
    // add more attributes as needed
    struct edge* next;
} Edge;

typedef struct node {
    int id;
    double lat;
    double lon;
    Edge* edges;
} Node;

typedef struct graph {
    int num_nodes;
    Node* nodes[MAX_NODES];
} Graph;

// Structure to represent a location
typedef struct {
    int id;
    char name[100];
} Location;

// Structure to represent an edge between two locations
typedef struct {
    int from;
    int to;
    int distance; // or time
} Edge;

// Graph represented as an adjacency list
typedef struct {
    Location locations[MAX_LOCATIONS];
    int num_locations;
    Edge edges[MAX_LOCATIONS * (MAX_LOCATIONS - 1) / 2];
    int num_edges;
} Graph;

#endif