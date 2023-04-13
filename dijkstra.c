#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <edge.h>

// Dijkstra's algorithm to find the shortest path
void dijkstra(Graph *graph, int start, int end) {
    int dist[MAX_LOCATIONS];
    int prev[MAX_LOCATIONS];
    int visited[MAX_LOCATIONS] = {0};

    // Initialize distances and previous nodes
    for (int i = 0; i < graph->num_locations; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;

    // Main loop
    for (int i = 0; i < graph->num_locations; i++) {
        // Find node with smallest tentative distance
        int min_dist = INT_MAX;
        int current = -1;
        for (int j = 0; j < graph->num_locations; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                current = j;
            }
        }

        if (current == -1) {
            break; // No reachable nodes left
        }

        visited[current] = 1;

        // Update distances of neighbors
        for (int j = 0; j < graph->num_edges; j++) {
            Edge edge = graph->edges[j];
            if (edge.from == current) {
                int neighbor = edge.to;
                int weight = edge.distance; // or time
                if (dist[current] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[current] + weight;
                    prev[neighbor] = current;
                }
            }
        }
    }

    // Print shortest path
    if (prev[end] == -1) {
        printf("No path found.\n");
    } else {
        int path[MAX_LOCATIONS];
        int num_nodes = 0;
        for (int i = end; i != -1; i = prev[i]) {
            path[num_nodes++] = i;
        }
        printf("Shortest path from %s to %s: ", graph->locations[start].name, graph->locations[end].name);
        for (int i = num_nodes - 1; i >= 0; i--) {
            printf("%s", graph->locations[path[i]].name);
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf(" (distance = %d)\n", dist[end]);
    }
}

int main() {
    Graph graph;
    graph.num
