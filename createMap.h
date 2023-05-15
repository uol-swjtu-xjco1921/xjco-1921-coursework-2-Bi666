#include <stdio.h>
#include <stdlib.h>
#include "map.h"

void init_map(map_t* map);
int add_node(map_t *map, int id, double lat, double lon);
int add_edge(map_t *map, int id, int node1, int node2, double length, double veg, double arch, double land);
int add_way(map_t *map, int id, int count, int nodes[MAX_WAY]);
int add_geom(map_t *map, int id, int count, int nodes[MAX_WAY]);
void add_speed(map_t *map);
void freeMap(map_t* map, range_t *bound, path_t *path);