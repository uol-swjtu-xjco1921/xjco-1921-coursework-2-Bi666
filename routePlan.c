#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "readFile.h"
#include "createMap.h"

int main(int argc, char **argv)
{
    map_t* map = malloc(sizeof(map_t));
    init_map(map);

    // Load map from file
    char *filename = argv[1];
    readMap(filename, map);

    // Find best path between two locations
    int start_node_id, end_node_id;
    printf("Enter start node ID: ");
    scanf("%d", &start_node_id);
    printf("Enter end node ID: ");
    scanf("%d", &end_node_id);

    node_t* start_node = NULL;
    node_t* end_node = NULL;

    for (int i = 0; i < map->num_nodes; i++)
    {
        if (map->nodes[i].id == start_node_id)
        {
            start_node = &map->nodes[i];;
        }
        if (map->nodes[i].id == end_node_id)
        {
            end_node = &map->nodes[i];;
        }
    }

    if (start_node == NULL || end_node == NULL)
    {
        printf("Error: Invalid node ID\n");
        free_map(map);
        return 1;
    }

    double distances[MAX_NODES];
    int previous_nodes[MAX_NODES];
    bool visited[MAX_NODES];

    for (int i = 0; i < map->num_nodes; i++)
    {
        distances[i] = INFINITY;
        visited[i] = false;
    }

    distances[start_node->id] = 0;

    for (int i = 0; i < map->num_nodes; i++)
    {
        // Find unvisited node with smallest distance
        int current_node_id = -1;
        double current_distance = INFINITY;
        for (int j = 0; j < map->num_nodes; j++)
        {
            if (!visited[j] && distances[j] < current_distance)
            {
                current_node_id = j;
                current_distance = distances[j];
            }
        }

        if (current_node_id == -1)
        {
            break;
        }

        visited[current_node_id] = true;

        // Update distances to adjacent nodes
        for (int j = 0; j < map->nodes[current_node_id].num_edges; j++)
        {
            edge_t edge = map->nodes[current_node_id].edges[j];
            int adjacent_node_id = edge.node1 == current_node_id ? edge.node2 : edge.node1;
            double distance;
            if (distances[current_node_id] + edge.length < distances[adjacent_node_id])
            {
                distances[adjacent_node_id] = distances[current_node_id] + edge.length;
                previous_nodes[adjacent_node_id] = current_node_id;
            }
        }
    }

    if (distances[end_node->id] == INFINITY)
    {
        printf("No path found between start and end nodes\n");
        free_map(map);
        return 1;
    }

    // Traverse path and print output
    int current_node_id = end_node->id;
    int path[MAX_NODES];
    int path_length = 0;

    while (current_node_id != start_node->id)
    {
        path[path_length] = current_node_id;
        path_length++;
        current_node_id = previous_nodes[current_node_id];
    }

    path[path_length] = start_node->id;
    path_length++;

    printf("Shortest path from node %d to node %d:\n", start_node->id, end_node->id);
    printf("Length: %.2f\n", distances[end_node->id]);

    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d ", path[i]);
        if (i > 0)
        {
            printf("-> ");
        }
    }
    printf("\n");
    // Free all allocated memory
    free_map(map);

    return 0;
}