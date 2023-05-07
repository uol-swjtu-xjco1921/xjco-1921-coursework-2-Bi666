#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int hash(int key, int size);
hashtable_t* hashtable_create(int size);
void hashtable_put(hashtable_t* hashtable, int key, node_t* value);
node_t* hashtable_get(hashtable_t* hashtable, int key);
void dijkstra(map_t* map, int start_id, int end_id);
