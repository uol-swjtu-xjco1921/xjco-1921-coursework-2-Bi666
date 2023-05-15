#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "map.h"

void init_map(map_t *map)
{
    map->nodes = NULL;
    map->num_nodes = 0;
    map->num_edges = 0;
    map->ways = NULL;
    map->num_ways = 0;
}

// Add node to map
int add_node(map_t *map, int id, double lat, double lon)
{
    if (map->num_nodes >= MAX_NODES)
    {
        printf("Error: Maximum number of nodes exceeded\n");
        return EXIT_EXCEED_RANGE;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        printf("Error: malloc failed\n");
        return EXIT_MALLOC_FAILED;
    }
    new_node->id = id;
    new_node->lat = lat;
    new_node->lon = lon;
    new_node->edges = NULL;
    new_node->num_edges = 0;
    new_node->next = NULL;
    new_node->count = map->num_nodes;

    // Add the new node to the end of the node list
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
    // Update the number of nodes in the map
    map->num_nodes++;
    return EXIT_NO_ERRORS;
}

int add_edge(map_t *map, int id, int node1, int node2, double length, double veg, double arch, double land)
{
    edge_t *new_edge = (edge_t *)malloc(sizeof(edge_t));
    if (new_edge == NULL)
    {
        printf("Error: malloc failed\n");
        return EXIT_MALLOC_FAILED;
    }
    new_edge->id = id;
    new_edge->node1 = node1;
    new_edge->node2 = node2;
    new_edge->length = length;
    new_edge->veg = veg;
    new_edge->arch = arch;
    new_edge->land = land;
    new_edge->speed = 0;
    new_edge->next = NULL;
    // Add the new edge to the list of edges of the starting node
    node_t *current_node = map->nodes;
    while (current_node != NULL && current_node->id != node1)
    {
        current_node = current_node->next;
    }
    if (current_node == NULL)
    {
        // The starting node does not exist, and the edge cannot be added
        printf("Error: no such point\n");
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
        // End node does not exist, cannot add edge
        printf("Error: no such point\n");
        return EXIT_BAD_DATA;
    }
    current_node->num_edges++;

    // Update the number of edges in the map
    map->num_edges++;
    return EXIT_NO_ERRORS;
}

// Add way to map
int add_way(map_t *map, int id, int count, int nodes[MAX_WAY])
{
    way_t *new_way = (way_t *)malloc(sizeof(way_t));
    if (new_way == NULL)
    {
        printf("Error: malloc failed\n");
        return EXIT_MALLOC_FAILED;
    }
    new_way->id = id;
    new_way->node_count = count;
    new_way->speed_limit = 0;
    for (int i = 0; i < count; i++)
    {
        new_way->node[i] = nodes[i];
    }
    new_way->next = NULL; // Initialize the 'next' pointer
    // Add the new node to the end of the node list
    if (map->ways == NULL)
    {
        map->ways = new_way;
    }
    else
    {
        way_t *current_way = map->ways;
        while (current_way->next != NULL)
        {
            current_way = current_way->next;
        }
        current_way->next = new_way;
    }
    // Update the number of nodes in the map
    map->num_ways++;
    return EXIT_NO_ERRORS;
}

// Add geom to map
int add_geom(map_t *map, int id, int count, int nodes[MAX_WAY])
{
    geom_t *new_geom = (geom_t *)malloc(sizeof(geom_t));
    if (new_geom == NULL)
    {
        printf("Error: malloc failed\n");
        return EXIT_MALLOC_FAILED;
    }
    new_geom->id = id;
    new_geom->node_count = count;
    for (int i = 0; i < count; i++)
    {
        new_geom->node[i] = nodes[i];
    }
    new_geom->next = NULL; // Initialize the 'next' pointer
    // Add the new node to the end of the node list
    if (map->geoms == NULL)
    {
        map->geoms = new_geom;
    }
    else
    {
        geom_t *current_geom = map->geoms;
        while (current_geom->next != NULL)
        {
            current_geom = current_geom->next;
        }
        current_geom->next = new_geom;
    }
    // Update the number of nodes in the map
    map->num_geoms++;
    return EXIT_NO_ERRORS;
}

void add_speed(map_t *map)
{
    way_t *current_way = map->ways;
    // Set random number seed
    srand((unsigned)time(NULL));
    while (current_way->next != NULL)
    {
        // Generate random speed limits
        int speed_li = (rand() % 50) + 30;
        current_way->speed_limit = speed_li;
        for (int i = 0; i < current_way->node_count - 1; i++)
        {
            int node1 = current_way->node[i];
            int node2 = current_way->node[i + 1];
            node_t *node = map->nodes;
            while (node != NULL)
            {
                if (node->id == node1 || node->id == node2)
                {
                    edge_t *edge = node->edges;
                    while (edge != NULL)
                    {
                        if ((edge->node1 == node1 && edge->node2 == node2) || (edge->node1 == node2 && edge->node2 == node1))
                        {
                            edge->speed = speed_li;
                            break;
                        }
                        edge = edge->next;
                    }
                }
                node = node->next;
            }
        }
        current_way = current_way->next;
    }
}

// Free memory
void freeMap(map_t *map, range_t *bound, path_t *path)
{
    if (map == NULL)
    {
        return;
    }
    // Freeing the memory of a node
    node_t *currentNode = map->nodes;
    while (currentNode != NULL)
    {
        edge_t *currentEdge = currentNode->edges;
        while (currentEdge != NULL)
        {
            edge_t *nextEdge = currentEdge->next;
            free(currentEdge);
            currentEdge = nextEdge;
        }
        node_t *nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }

    // Freeing the memory of a way
    way_t *currentWay = map->ways;
    while (currentWay != NULL)
    {
        way_t *nextWay = currentWay->next;
        free(currentWay);
        currentWay = nextWay;
    }

    // Freeing the memory of a geom
    geom_t *currentGeom = map->geoms;
    while (currentGeom != NULL)
    {
        geom_t *nextGeom = currentGeom->next;
        free(currentGeom);
        currentGeom = nextGeom;
    }

    // Freeing the memory of map
    free(map);
    map = NULL;

    // free bound
    if (bound != NULL)
    {
        free(bound);
        bound = NULL;
    }
    // free path
    if (path != NULL)
    {
        free(path);
        path = NULL;
    }
}