#include <stdio.h>
#include <stdlib.h>
#include <data.h>

node_t read_node(char* line) {
    node_t node;
    sscanf(line, "%d,%lf,%lf", &node.id, &node.lat, &node.lon);
    return node;
}

link_t read_link(char* line) {
    link_t link;
    sscanf(line, "%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf", &link.id, &link.node1, &link.node2, &link.way, &link.length, &link.veg, &link.arch, &link.land, &link.POI);
    return link;
}

adjacency_list_t* build_adjacency_list(char* filename) {
    FILE* file = fopen(filename, "r");
    char line[1024];
    node_t* nodes = NULL;
    int num_nodes = 0;
    link_t* links = NULL;
    int num_links = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'n') {
            node_t node = read_node(line);
            nodes = realloc(nodes, sizeof(node_t) * (num_nodes + 1));
            nodes[num_nodes++] = node;
        } else if (line[0] == 'l') {
            link_t link = read_link(line);
            links = realloc(links, sizeof(link_t) * (num_links + 1));
            links[num_links++] = link;
        }
    }

    fclose(file);

    adjacency_list_t* adj_list = malloc(sizeof(adjacency_list_t));
    adj_list->nodes = nodes;
    adj_list->num_nodes = num_nodes;
    adj_list->links = links;
    adj_list->num_links = num_links;
    return adj_list;
}
