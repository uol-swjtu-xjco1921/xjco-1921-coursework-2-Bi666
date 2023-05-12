#include <stdio.h>
#include <stdlib.h>

#ifndef MAP_H
#define MAP_H

#define MAX_NODES 10000
#define MAX_LINE_LENGTH 10000
#define DIS_MAX 2147483647.0
#define MAX_WAY 100 // 假设每条路最多有 100 个节点

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_FILE_NAME 2
#define EXIT_MALLOC_FAILED 3
#define EXIT_BAD_DATA 4
#define EXIT_EXCEED_RANGE 5
#define EXIT_NO_PATH_FOUND 6
#define EXIT_OUTPUT_FAILED 7

/* Define data structures */
typedef struct path_t {
    int count;
    int *pathCount;
} path_t;

typedef struct range {
    double minLat;
    double minLon;
    double maxLat;
    double maxLon;
} range;

typedef struct edge_t {
    int id;
    int node1;
    int node2;
    double length;
    double veg;
    double arch;
    double land;
    int speed;
    struct edge_t* next;
} edge_t;

typedef struct node_t {
    int id;
    double lat;
    double lon;
    struct edge_t* edges;
    int num_edges;
    struct node_t* next;
    int count;
} node_t;

typedef struct way_t {
    int id;
    int node[MAX_WAY];
    int node_count;
    int speed_limit;
    struct way_t* next;
} way_t;

typedef struct map_t {
    node_t* nodes;
    int num_nodes;
    int num_edges;
    way_t* ways;
    int num_ways;
} map_t;

typedef struct sizeMap_t {
    double latRange;
    double lonRange;
    double xRatio;
    double yRatio;
    double xOffset;
    double yOffset;
    int NODE_SIZE;
} sizeMap_t;

#endif
