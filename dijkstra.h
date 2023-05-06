#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int get_shortest_node(double* distances, bool* visited, int num_nodes);
int dijkstra(map_t *map, node_t *start_node, node_t *end_node);