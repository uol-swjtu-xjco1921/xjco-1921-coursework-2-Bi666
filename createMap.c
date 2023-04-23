#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"

void init_map(map_t* map)
{
    map->nodes = NULL;
    map->num_nodes = 0;
    map->num_edges = 0;
}

// Add node to map
void add_node(map_t* map, int id, double lat, double lon) {
    if (map->num_nodes >= MAX_NODES)
    {
        printf("Error: Maximum number of nodes exceeded\n");
        exit(1);
    }
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        exit(1);
    }
    new_node->id = id;
    new_node->lat = lat;
    new_node->lon = lon;
    new_node->edges = NULL;
    new_node->num_edges = 0;
    new_node->next = map->nodes;
    map->nodes = new_node;
    map->num_nodes++;
}

void add_edge(map_t* map, int id, int node1, int node2, double length) {
    edge_t* new_edge = (edge_t*)malloc(sizeof(edge_t));
    if (new_edge == NULL) {
        exit(1);
    }
    new_edge->id = id;
    new_edge->node1 = node1;
    new_edge->node2 = node2;
    new_edge->length = length;
    new_edge->next = NULL;

    node_t* node = map->nodes;
    while (node != NULL) {
        if (node->id == node1 || node->id == node2) {
            edge_t* node_edge = (edge_t*)malloc(sizeof(edge_t));
            *node_edge = *new_edge;
            node_edge->next = node->edges;
            node->edges = node_edge;
            node->num_edges++;
        }
        node = node->next;
    }
    map->num_edges++;
}

double get_distance(node_t *node1, node_t *node2)
{
    // Calculate the distance between two nodes using the Haversine formula
    double lat1 = node1->lat;
    double lon1 = node1->lon;
    double lat2 = node2->lat;
    double lon2 = node2->lon;
    double dlat = (lat2 - lat1) * M_PI / 180.0;
    double dlon = (lon2 - lon1) * M_PI / 180.0;
    double a = pow(sin(dlat / 2), 2) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = 6371 * c;
    return distance;
}

void free_map(map_t* map) {
    node_t* node = map->nodes;
    while (node != NULL) {
        edge_t* edge = node->edges;
        while (edge != NULL) {
            edge_t* temp_edge = edge;
            edge = edge->next;
            free(temp_edge);
        }
        node_t* temp_node = node;
        node = node->next;
        free(temp_node);
    }
    free(map);
    map->nodes = NULL;
    map->num_nodes = 0;
    map->num_edges = 0;
}
