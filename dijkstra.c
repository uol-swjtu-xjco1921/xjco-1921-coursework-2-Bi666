#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "map.h"

int get_node_count(map_t *map, int nodeID)
{
    node_t *node = map->nodes;
        while (node != NULL)
        {
            if (node->id == nodeID) break;
            node = node->next;
        }
    return node->count;
}

// 获取未访问节点中距离最短的节点
int get_shortest_node(double* distances, bool* visited, int num_nodes)
{
    int i, shortest_node = -1;
    int shortest_distance = DIS_MAX;

    for (i = 0; i < num_nodes; i++)
    {
        if (!visited[i] && distances[i] < shortest_distance)
        {
            shortest_node = i;
            shortest_distance = distances[i];
        }
    }
    return shortest_node;
}

int dijkstra(map_t *map, node_t *start_node, node_t *end_node)
{
    double *distances;
    int *previous_nodes;
    bool *visited;

    distances = (double *)malloc(map->num_nodes * sizeof(double));
    previous_nodes = (int *)malloc(map->num_nodes * sizeof(int));
    visited = (bool *)malloc(map->num_nodes * sizeof(bool));

    if (distances == NULL || previous_nodes == NULL || visited == NULL)
    {
        printf("Error: malloc failed\n");
        if (distances != NULL)
            free(distances);
        if (previous_nodes != NULL)
            free(previous_nodes);
        if (visited != NULL)
            free(visited);
        return EXIT_MALLOC_FAILED;
    }

    for (int i = 0; i < map->num_nodes; i++)
    {
        previous_nodes[i] = -1;
        distances[i] = DIS_MAX;
        visited[i] = false;
    }

    distances[start_node->count] = 0;

    for (int i = 0; i < map->num_nodes - 1; i++)
    {
        int current = get_shortest_node(distances, visited, map->num_nodes);
        if (current == -1) 
            break;
        visited[current] = true;

        node_t *node = map->nodes;
        while (node != NULL)
        {
            if (node->count == current) break;
            node = node->next;
        }

        edge_t *edge = node->edges;
        while (edge != NULL)
        {
            int neighbor = get_node_count(map, edge->node2);
            if (!visited[neighbor])
            {
                double new_distance = distances[current] + edge->length;

                if (new_distance < distances[neighbor])
                {
                    distances[neighbor] = new_distance;
                    previous_nodes[neighbor] = node->count;
                }
            }
            edge = edge->next;
        }
        node_t *findnode = map->nodes;
        while (findnode != NULL)
        {
            if (findnode->count == node->count)
            {
                findnode = findnode->next;
                continue;
            }
            int neighbor = -1;
            edge_t *findedge = findnode->edges;
            while (findedge != NULL)
            {
                if (node->id == findedge->node2) 
                {
                    neighbor = get_node_count(map, findedge->node1);
                    break;
                }
                findedge = findedge->next;
            }
            if (neighbor != -1 && !visited[neighbor])
            {
                double new_distance = distances[current] + findedge->length;

                if (new_distance < distances[neighbor])
                {
                    distances[neighbor] = new_distance;
                    previous_nodes[neighbor] = node->count;
                }
            }
            findnode = findnode->next;
        }
    }

    if (distances[end_node->count] == DIS_MAX)
    {
        printf("No path found between start and end nodes\n");
        free(distances);
        free(previous_nodes);
        free(visited);
        return EXIT_NO_PATH_FOUND;
    }
    
    // Traverse path and print output
    int path[map->num_nodes], count = 0;
    int nodeCount = end_node->count;

    while (nodeCount != start_node->count) {
        path[count] = nodeCount;
        count++;
        nodeCount = previous_nodes[nodeCount];
    }

    path[count] = start_node->count;
    count++;

    for (int i = count - 1; i >= 0; i--) {
        printf("%d ", path[i]);
        if (i != 0) {
            printf("-> ");
        }
    }
    printf("\n");

    free(distances);
    free(previous_nodes);
    free(visited);
    
    return EXIT_NO_ERRORS;
}