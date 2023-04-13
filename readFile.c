#include <stdio.h>
#include <stdlib.h>
#include <edge.h>

Graph* create_graph() {
    Graph* graph = malloc(sizeof(Graph));
    graph->num_nodes = 0;
    return graph;
}

Node* create_node(int id, double lat, double lon) {
    Node* node = malloc(sizeof(Node));
    node->id = id;
    node->lat = lat;
    node->lon = lon;
    node->edges = NULL;
    return node;
}

Edge* create_edge(int node_id, double distance, double speed_limit) {
    Edge* edge = malloc(sizeof(Edge));
    edge->node_id = node_id;
    edge->distance = distance;
    edge->speed_limit = speed_limit;
    edge->next = NULL;
    return edge;
}

void add_edge(Node* node, Edge* edge) {
    edge->next = node->edges;
    node->edges = edge;
}

void add_node(Graph* graph, Node* node) {
    graph->nodes[graph->num_nodes++] = node;
}

void read_map_data(char* filename, Graph* graph) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int type, id1, id2;
        double lat, lon, distance, speed_limit;
        // read the data from the file
        sscanf(line, "%d,%d,%d,%lf,%lf,%lf,%lf", &type, &id1, &id2, &lat, &lon, &distance, &speed_limit);

        if (type == 1) { // node
            Node* node = create_node(id1, lat, lon);
            add_node(graph, node);
        } else if (type == 2) { // link
            Node* node1 = graph->nodes[id1];
            Node* node2 = graph->nodes[id2];
            double dist = /* compute distance between node1 and node2 */;
            Edge* edge1 = create_edge(id2, dist, speed_limit);
            Edge* edge2 = create_edge(id1, dist, speed_limit);
            add_edge(node1, edge1);
            add_edge(node2, edge2);
        }
    }

    fclose(fp);
}
