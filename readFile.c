#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "createMap.h"

int readMap(char *filename, map_t *map, range *bound)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file '%s'\n", filename);
        return EXIT_BAD_FILE_NAME;
    }

    char buffer[1000];
    //Read node
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Parse data line
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'n')
        {
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL)
            {
                continue; // ignore lines without an ID
            }
            int id = atoi(id_str);
            strtok(NULL, "=");
            char *lat_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *lon_str = strtok(NULL, " ");
            double lat = 0.0;
            double lon = 0.0;
            if (lat_str != NULL && lon_str != NULL)
            {
                lat = atof(lat_str);
                lon = atof(lon_str);
            }
            else
            {
                printf("Error: Invalid data for node %d\n", id);
                free_map(map);
                return EXIT_BAD_DATA;
            }
            // if (lat < bound->minLat || lat > bound->maxLat || lon < bound->minLon || lon > bound->maxLon)
            // {
            //     printf("Error: Invalid data for node %d\n", id);
            //     free_map(map);
            //     return EXIT_BAD_DATA;
            // }
            // Create new node and add to map
            int addresult = add_node(map, id, lat, lon);
            if (addresult != EXIT_NO_ERRORS)
            {
                return addresult;
            }
        }
        else if (tag != NULL && tag[1] == 'b')
        {
            strtok(NULL, "=");
            char *minlat_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *minlon_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *maxlat_str = strtok(NULL, " ");
            strtok(NULL, "=");
            char *maxlon_str = strtok(NULL, " ");
            if (minlat_str == NULL || minlon_str == NULL || maxlat_str == NULL || maxlon_str == NULL)
            {
                printf("Error: Invalid data for bounding");
                free_map(map);
                return EXIT_BAD_DATA;
            }
            bound->minLat = atof(minlat_str);
            bound->minLon = atof(minlon_str);
            bound->maxLat = atof(maxlat_str);
            bound->maxLon = atof(maxlon_str);
        }
    }

    //Go back to the head of file
    fseek(fp, 0, SEEK_SET);
    //Read link
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'l')
        {
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL)
            {
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
            if (node1_str == NULL || node2_str == NULL || length_str == NULL
                || way_str == NULL || veg_str == NULL || arch_str == NULL || land_str == NULL)
            {
                printf("Error: Invalid data for link %d\n", id);
                free_map(map);
                return EXIT_BAD_DATA;
            }
            int node1 = atoi(node1_str);
            int node2 = atoi(node2_str);
            int way = atoi(way_str);
            double length = atof(length_str);
            double veg = atof(veg_str);
            double arch = atof(arch_str);
            double land = atof(land_str);
            // Create new edge and add to nodes
            int addresult = add_edge(map, id, node1, node2, length, veg, arch, land);
            if (addresult != EXIT_NO_ERRORS)
            {
                return addresult;
            }
        }
    }
    fclose(fp);
    return EXIT_NO_ERRORS;
}