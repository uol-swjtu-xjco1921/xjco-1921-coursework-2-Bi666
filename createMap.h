#include <stdio.h>
#include "map.h"

void init_map(map_t* map);
void add_node(map_t *map, int id, double lat, double lon);
void add_edge(map_t* map, int id, int node1, int node2, double length);
double get_distance(node_t *node1, node_t *node2);
void free_map(map_t *map);