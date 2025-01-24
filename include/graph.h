#pragma once


typedef struct {
    int nodeCount;
    int* nodes;
    int* edges;
} Graph;

Graph GraphNew(int nodeCount);
int GraphAdd(Graph* graph, int node1, int node2);
int GraphRemove(Graph* graph, int node1, int node2);
int GraphNode(Graph* graph, int node);
int GraphVisitNode(Graph* graph, int node);
int GraphConnectedTo(Graph* graph, int node, int* nodes);
void GraphDestroy(Graph* graph);
void GraphPrint(Graph* graph);
void GraphPrintD(Graph* graph);