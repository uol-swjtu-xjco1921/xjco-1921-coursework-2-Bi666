#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"

void init_map(map_t *map)
{
    map->nodes = NULL;
    map->num_nodes = 0;
    map->num_edges = 0;
}

void free_map(map_t* map) {
    // Free the nodes in the map
    node_t* node = map->nodes;
    while (node != NULL) {
        // Free the edges in each node
        edge_t* edge = node->edges;
        while (edge != NULL) {
            edge_t* temp = edge->next; // save the next edge pointer before freeing the current one
            free(edge);
            edge = temp; // move to the next edge
        }
        node_t* temp = node->next; // save the next node pointer before freeing the current one
        free(node);
        node = temp; // move to the next node
    }
    // Free the map
    free(map);
    map = NULL;
    // set to NULL to avoid dangling pointer
}

// Add node to map
int add_node(map_t *map, int id, double lat, double lon)
{
    if (map->num_nodes >= MAX_NODES)
    {
        printf("Error: Maximum number of nodes exceeded\n");
        free_map(map);
        return EXIT_EXCEED_RANGE;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        printf("Error: malloc failed\n");
        free_map(map);
        return EXIT_MALLOC_FAILED;
    }
    new_node->id = id;
    new_node->lat = lat;
    new_node->lon = lon;
    new_node->edges = NULL;
    new_node->num_edges = 0;
    new_node->next = NULL;
     new_node->count = map->num_nodes;
    // 将新节点添加到节点列表的末尾
    if (map->nodes == NULL)
    {
        map->nodes = new_node;
    }
    else
    {
        node_t *current_node = map->nodes;
        while (current_node->next != NULL)
        {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
    // 更新地图中节点的数量
    map->num_nodes++;
    return EXIT_NO_ERRORS;
}

int add_edge(map_t *map, int id, int node1, int node2, double length)
{
    edge_t *new_edge = (edge_t *)malloc(sizeof(edge_t));
    if (new_edge == NULL)
    {
        printf("Error: malloc failed\n");
        free_map(map);
        return EXIT_MALLOC_FAILED;
    }#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"

void init_map(map_t *map)
{
    map->nodes = NULL;
    map->num_nodes = 0;
    map->num_edges = 0;
}

void free_map(map_t* map) {
    // Free the nodes in the map
    node_t* node = map->nodes;
    while (node != NULL) {
        // Free the edges in each node
        edge_t* edge = node->edges;
        while (edge != NULL) {
            edge_t* temp = edge->next; // save the next edge pointer before freeing the current one
            free(edge);
            edge = temp; // move to the next edge
        }
        node_t* temp = node->next; // save the next node pointer before freeing the current one
        free(node);
        node = temp; // move to the next node
    }
    // Free the map
    free(map);
    map = NULL;
    // set to NULL to avoid dangling pointer
}

// Add node to map
int add_node(map_t *map, int id, double lat, double lon)
{
    if (map->num_nodes >= MAX_NODES)
    {
        printf("Error: Maximum number of nodes exceeded\n");
        free_map(map);
        return EXIT_EXCEED_RANGE;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        printf("Error: malloc failed\n");
        free_map(map);
        return EXIT_MALLOC_FAILED;
    }
    new_node->id = id;
    new_node->lat = lat;
    new_node->lon = lon;
    new_node->edges = NULL;
    new_node->num_edges = 0;
    new_node->next = NULL;
     new_node->count = map->num_nodes;
    // 将新节点添加到节点列表的末尾
    if (map->nodes == NULL)
    {
        map->nodes = new_node;
    }
    else
    {
        node_t *current_node = map->nodes;
        while (current_node->next != NULL)
        {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
    // 更新地图中节点的数量
    map->num_nodes++;
    return EXIT_NO_ERRORS;
}

int add_edge(map_t *map, int id, int node1, int node2, double length)
{
    edge_t *new_edge = (edge_t *)malloc(sizeof(edge_t));
    if (new_edge == NULL)
    {
        printf("Error: malloc failed\n");
        free_map(map);
        return EXIT_MALLOC_FAILED;
    }
    new_edge->id = id;
    new_edge->node1 = node1;
    new_edge->node2 = node2;
    new_edge->length = length;
    new_edge->next = NULL;
    // 将新边添加到起始节点的边列表中
    node_t *current_node = map->nodes;
    while (current_node != NULL && current_node->id != node1)
    {
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        // 起始节点不存在，无法添加边
        printf("Error: no such point\n");
        free_map(map);
        return EXIT_BAD_DATA;
    }
    if (current_node->edges == NULL)
    {
        current_node->edges = new_edge;
    }
    else
    {
        edge_t *current_edge = current_node->edges;
        while (current_edge->next != NULL)
        {
            current_edge = current_edge->next;
        }
        current_edge->next = new_edge;
    }
    current_node->num_edges++;

    current_node = map->nodes;
    while (current_node != NULL && current_node->id != node2)
    {
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        // 结束节点不存在，无法添加边
        printf("Error: no such point\n");
        free_map(map);
        return EXIT_BAD_DATA;
    }
    current_node->num_edges++;

    // 更新地图中边的数量
    map->num_edges++;
    return EXIT_NO_ERRORS;
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
    new_edge->id = id;
    new_edge->node1 = node1;
    new_edge->node2 = node2;
    new_edge->length = length;
    new_edge->next = NULL;
    // 将新边添加到起始节点的边列表中
    node_t *current_node = map->nodes;
    while (current_node != NULL && current_node->id != node1)
    {
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        // 起始节点不存在，无法添加边
        printf("Error: no such point\n");
        free_map(map);
        return EXIT_BAD_DATA;
    }
    if (current_node->edges == NULL)
    {
        current_node->edges = new_edge;
    }
    else
    {
        edge_t *current_edge = current_node->edges;
        while (current_edge != NULL)
        {
            current_edge = current_edge->next;
        }
        current_edge = new_edge;
    }
    current_node->num_edges++;

    current_node = map->nodes;
    while (current_node != NULL && current_node->id != node2)
    {
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        // 结束节点不存在，无法添加边
        printf("Error: no such point\n");
        free_map(map);
        return EXIT_BAD_DATA;
    }
    current_node->num_edges++;

    // 更新地图中边的数量
    map->num_edges++;
    return EXIT_NO_ERRORS;
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