#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "createMap.h"

int readMap(char *filename, map_t *map, range_t *bound)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("ERROR: Bad File Name (%s)\n", filename);
        return EXIT_BAD_FILE_NAME;
    }

    int num_node = 0;
    int num_link = 0;
    int num_way = 0;
    int num_bound = 0;
    char buffer[1000];
    // Read node
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Parse data line
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'n')
        {
            num_node++;
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
                printf("ERROR: Bad Attributes (%s)", filename);
                fclose(fp);
                return EXIT_BAD_DATA;
            }
            // if (lat < bound->minLat || lat > bound->maxLat || lon < bound->minLon || lon > bound->maxLon)
            // {
            //     printf("Error: Invalid data for node %d\n", id);
            //     return EXIT_BAD_DATA;
            // }
            // Create new node and add to map
            int addresult = add_node(map, id, lat, lon);
            if (addresult != EXIT_NO_ERRORS)
            {
                fclose(fp);
                return addresult;
            }
        }
        else if (tag != NULL && tag[1] == 'b')
        {
            num_bound++;
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
                printf("ERROR: Bad Attributes (%s)", filename);
                fclose(fp);
                return EXIT_BAD_DATA;
            }
            bound->minLat = atof(minlat_str) - 0.001;
            bound->minLon = atof(minlon_str);
            bound->maxLat = atof(maxlat_str) + 0.002;
            bound->maxLon = atof(maxlon_str) + 0.004;
        }
    }

    // Go back to the head of file
    fseek(fp, 0, SEEK_SET);
    // Read link
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'l')
        {
            num_link++;
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
            if (node1_str == NULL || node2_str == NULL || length_str == NULL || way_str == NULL || veg_str == NULL || arch_str == NULL || land_str == NULL)
            {
                printf("ERROR: Bad Attributes (%s)", filename);
                fclose(fp);
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
                fclose(fp);
                return addresult;
            }
        }
        if (tag != NULL && tag[1] == 'w')
        {
            num_way++;
            int count = 0;
            int nodeid[MAX_WAY];
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL)
            {
                continue; // ignore lines without an ID
            }
            int id = atoi(id_str);
            while (strcmp(strtok(NULL, "="), "node") == 0)
            {
                char *node_str = strtok(NULL, " ");
                if (node_str == NULL)
                {
                    printf("ERROR: Bad Attributes (%s)", filename);
                    fclose(fp);
                    return EXIT_BAD_DATA;
                }
                nodeid[count] = atoi(node_str);
                count++;
            }
            add_way(map, id, count, nodeid);
        }
        if (tag != NULL && tag[1] == 'g')
        {
            int count = 0;
            int nodeid[MAX_WAY];
            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            if (id_str == NULL)
            {
                continue; // ignore lines without an ID
            }
            int id = atoi(id_str);
            while (strcmp(strtok(NULL, "="), "node") == 0)
            {
                char *node_str = strtok(NULL, " ");
                if (node_str == NULL)
                {
                    printf("ERROR: Bad Attributes (%s)", filename);
                    fclose(fp);
                    return EXIT_BAD_DATA;
                }
                nodeid[count] = atoi(node_str);
                count++;
            }
            add_geom(map, id, count, nodeid);
        }
    }
    // Determine if the necessary labels for the map are complete
    if (num_bound == 0 | num_link == 0 | num_node == 0 | num_way == 0)
    {
        printf("ERROR: Bad Labels (%s)\n", filename);
        fclose(fp);
        return EXIT_OUTPUT_FAILED;
    }

    // Add random speed attributes
    add_speed(map);

    FILE *outputFile;
    char tempFilename[] = "newmap.map";
    outputFile = fopen(tempFilename, "w");
    if (outputFile == NULL) {
        printf("ERROR: Failed to write output (%s)\n", filename);
        fclose(fp);
        return EXIT_OUTPUT_FAILED;
    }
    
    // Go back to the head of file
    fseek(fp, 0, SEEK_SET);
    // Read link
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fputs(buffer, outputFile);
        char *tag = strtok(buffer, " ");
        if (tag != NULL && tag[1] == 'w')
        {

            strtok(NULL, "=");
            char *id_str = strtok(NULL, " ");
            int id = atoi(id_str);
            way_t *add_way = map->ways;
            while (add_way != NULL)
            {
                if (add_way->id == id)
                {
                    char n[100];
                    sprintf(n, "<add speed=%d /add>\n", add_way->speed_limit);
                    fputs(n, outputFile);
                    break;
                }
                add_way = add_way->next;
            }
        }
    }

    fclose(fp);
    fclose(outputFile);

    //Delete original file
    remove(filename);

    // Rename the temporary file to the original file name
    if (rename(tempFilename, filename) != 0) {
        printf("Error: Unable to rename the file\n");
        return EXIT_OUTPUT_FAILED;
    }
    return EXIT_NO_ERRORS;
}
