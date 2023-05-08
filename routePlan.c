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
    path_t *path = (path_t *)malloc(sizeof(path_t));
    path->count = 0;
    path->pathCount = NULL;
    sdl(map, path, 0);

    bool choose = true;
    while (choose)
    {
        printf("Want to show more map details? (y/n): ");
        char more;
        scanf(" %c", &more);
        // Read and discard any remaining characters in the input buffer
        while (getchar() != '\n')
            ;

        if (more == 'Y' || more == 'y')
        {
            printf("Choose to show \"veg\", \"arch\" or \"land\": ");
            char option[5];
            scanf("%s", option);
            // Read and discard any remaining characters in the input buffer
            while (getchar() != '\n')
                ;

            if (strcmp(option, "veg") == 0)
                sdl(map, path, 1);
            else if (strcmp(option, "arch") == 0)
                sdl(map, path, 2);
            else if (strcmp(option, "land") == 0)
                sdl(map, path, 3);
            else
                printf("Sorry, that is not an option!\n");
        }
        else
        {
            choose = false;
        }
    }

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

    int result = dijkstra(map, start_node, end_node, path);
    if (result != EXIT_NO_ERRORS)
        return result;

    sdl(map, path, 0);
    // Free all allocated memory
    free_map(map);
    free(bound);

    return EXIT_NO_ERRORS;
}