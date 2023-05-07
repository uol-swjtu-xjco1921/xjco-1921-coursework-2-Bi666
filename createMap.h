#include <stdio.h>
#include <stdlib.h>
#include "map.h"

void init_map(map_t* map);
void free_map(map_t* map);
int add_node(map_t *map, int id, double lat, double lon);
int add_edge(map_t* map, int id, int node1, int node2, double length);
double get_distance(node_t *node1, node_t *node2);