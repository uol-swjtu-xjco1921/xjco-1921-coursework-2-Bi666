#include <stdio.h>
#include <data.h>

node_t read_node(char* line);
link_t read_link(char* line);
adjacency_list_t* build_adjacency_list(char* filename);