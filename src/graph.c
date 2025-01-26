#include "graph.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

Graph GraphNew(int nodeCount) {
    assert(nodeCount > 0);
    int* nodes = malloc(nodeCount * sizeof(int));
    int* edges = malloc(nodeCount * nodeCount * sizeof(int));
    for (int i = 0; i < nodeCount; ++i) {
        nodes[i] = 0;
    }
    for (int i = 0; i < nodeCount * nodeCount; ++i) {
        edges[i] = 0;
    }
    return (Graph) {
        nodeCount,
        nodes,
        edges
    };
}
int GraphAdd(Graph* graph, int node1, int node2) {
    int nodeCount = graph->nodeCount;
    int i1 = node1 + nodeCount * node2;
    int i2 = node2 + nodeCount * node1;
    graph->edges[i1] = 1;
    graph->edges[i2] = 1;
    return 1;
}
int GraphRemove(Graph* graph, int node1, int node2) {
    int nodeCount = graph->nodeCount;
    int i1 = node1 + nodeCount * node2;
    int i2 = node2 + nodeCount * node1;
    graph->edges[i1] = 0;
    graph->edges[i2] = 0;
    return 1;
}
inline void GraphDestroy(Graph* graph) {
    free(graph->edges);
    free(graph->nodes);
}
int GraphNode(Graph* graph, int node) {
    assert(node < graph->nodeCount);
    return graph->nodes[node];
}
int GraphVisitNode(Graph* graph, int node) {
    assert(node < graph->nodeCount);
    graph->nodes[node] = 1;
    return 1;
}
int GraphConnectedTo(Graph* graph, int node, int* nodes) {
    assert(node < graph->nodeCount);
    int idx = 0;
    for (int i = 0; i < graph->nodeCount; ++i) {
        if (graph->edges[i + node * graph->nodeCount] && !GraphNode(graph, i)) {
            nodes[idx++] = i;
        }
    }
    return idx;
}
void GraphPrint(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; ++i) {
        for (int j = 0; j < graph->nodeCount; ++j) {
            printf("%d ", graph->edges[j + i * graph->nodeCount]);
        }
        printf("\n");
    }
    printf("\n");
}
