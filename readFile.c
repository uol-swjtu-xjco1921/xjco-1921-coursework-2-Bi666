#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "createMap.h"

void readMap(char *filename, map_t *map)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }
    
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), fp) !=  NULL)
    {
        // Parse data line
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'n')
        {
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL) {
                continue; // ignore lines without an ID
            }
            int id = atoi(id_str);
            strtok(NULL, "=");
            char *lat_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *lon_str = strtok(NULL, " ");
            double lat = 0.0;
            double lon = 0.0;
            if (lat_str != NULL && lon_str != NULL) {
                lat = atof(lat_str);
                lon = atof(lon_str);
            }
            else {
                printf("Error: Invalid data for node %d\n", id);
                free_map(map);
                exit(1);
            }
            // Create new node and add to map
            add_node(map, id, lat, lon);
        }
        else if (tag != NULL && tag[1] == 'l')
        {
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL) {
                continue; // ignore lines without an ID
            }
            int id = atoi(id_str);
            strtok(NULL, "=");
            char *node1_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *node2_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *way_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *length_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *veg_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *arch_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *land_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *poi_str = strtok(NULL, ";");
            if (node1_str == NULL || node2_str == NULL || length_str == NULL || way_str == NULL) {
                printf("Error: Invalid data for link %d\n", id);
                free_map(map);
                exit(1);
            }
            int node1 = atoi(node1_str);
            int node2 = atoi(node2_str);
            int way = atoi(way_str);
            double length = atof(length_str);

            // Create new edge and add to nodes
            add_edge(map,id, node1, node2, length);
        }
    }
    fclose(fp);
}