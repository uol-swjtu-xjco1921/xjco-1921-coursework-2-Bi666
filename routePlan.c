#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "readFile.h"
#include "createMap.h"
#include "plotSDL.h"
#include "dijkstra.h"

int main(int argc, char **argv)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    init_map(map);
    range *bound = (range *)malloc(sizeof(range));

    // Load map from file
    char *filename = argv[1];
    int readresult = readMap(filename, map, bound);
    if (readresult != EXIT_NO_ERRORS)
    {
        return readresult;
    }

    sdl(map, bound);

    // Find best path between two locations
    int start_node_id = 0, end_node_id = 0;
    printf("Enter start node ID: ");
    scanf("%d", &start_node_id);
    printf("Enter end node ID: ");
    scanf("%d", &end_node_id);

    node_t *start_node = NULL;
    node_t *end_node = NULL;

    node_t *current_node = map->nodes;
    while (current_node != NULL)
    {
        if (current_node->id == start_node_id)
        {
            start_node = current_node;
        }
        if (current_node->id == end_node_id)
        {
            end_node = current_node;
        }
        current_node = current_node->next;
    }

    if (start_node == NULL || end_node == NULL)
    {
        printf("Error: Invalid node ID\n");
        free_map(map);
        free(bound);
        return EXIT_BAD_DATA;
    }

    int result = dijkstra(map, start_node, end_node);
    if (result != EXIT_NO_ERRORS)
        return result;

    // Free all allocated memory
    free_map(map);
    free(bound);

    return EXIT_NO_ERRORS;
}