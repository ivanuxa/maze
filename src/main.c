#include "raylib.h"
#include "graph.h"
#include "stack.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

const int screenWidth = 960;
const int screenHeight = 960;
const int mazeWidth = 800;
const int mazeHeight = 800;
const int n = 100;

void mazeOnRedraw(Graph* graph, Stack* stack) {
  const int offsetX = (screenWidth - mazeWidth) / 2;
  const int offsetY = (screenHeight - mazeHeight) / 2;
  const int cellWidth = mazeWidth / n;
  const int cellHeight = mazeHeight / n;
  DrawLine(offsetX, offsetY, offsetX, offsetY + mazeHeight, BLACK);
  DrawLine(offsetX, offsetY, offsetX + mazeWidth, offsetY, BLACK);
  DrawLine(offsetX + mazeWidth + 1, offsetY, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  DrawLine(offsetX, offsetY + mazeHeight + 1, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  int nodeCount = graph->nodeCount;
  for (int i = 0; i < nodeCount; ++i) {
    int x = i % n;
    int y = i / n;
    DrawRectangle(offsetX + x * cellWidth, offsetY + y * cellHeight, cellWidth, cellHeight, GraphNode(graph, i) ? RAYWHITE : BLUE);
    if (x != 0 && graph->edges[i - 1 + i * nodeCount]) {
      DrawLine(offsetX + x * cellWidth, offsetY + y * cellHeight, offsetX + x * cellWidth, offsetY + y * cellHeight + cellHeight, BLACK);
    }
    if (x != n - 1 && graph->edges[i + 1 + i * nodeCount]) {
      DrawLine(offsetX + x * cellWidth + cellWidth, offsetY + y * cellHeight, offsetX + x * cellWidth + cellWidth, offsetY + y * cellHeight + cellHeight, BLACK);
    }
    if (y != 0 && graph->edges[i - n + i * nodeCount]) {
      DrawLine(offsetX + x * cellWidth, offsetY + y * cellHeight, offsetX + x * cellWidth + cellWidth, offsetY + y * cellHeight, BLACK);
    }
    if (y != n - 1 && graph->edges[i + n + i * nodeCount]) {
      DrawLine(offsetX + x * cellWidth, offsetY + y * cellHeight + cellHeight, offsetX + x * cellWidth + cellWidth, offsetY + y * cellHeight + cellHeight, BLACK);
    }
  }
  if (stack->size > 0) {
    int node = StackPeek(stack);
    int x = node % n;
    int y = node / n;
    DrawRectangle(offsetX + x * cellWidth, offsetY + y * cellHeight, cellWidth, cellHeight, RED);
  }
}

void reevalGraph(Graph* graph, Stack* stack) {
  /*
  1. if stack is empty -> do nothing
  2. Peek a stack
  3. Color a node
  4. Count up not colored neighbours
  5. If there is none -> pop the stack, go to 1
  6. If there is one -> go to 8
  7. If there is a few -> pick at random
  8. Add to stack picked neighbour
  9. Remove an edge 
  */
  
  if (stack->size == 0) {
    return;
  }
  int node = StackPeek(stack);
  GraphVisitNode(graph, node);
  int connectedNodes[4];
  int connectedSize = GraphConnectedTo(graph, node, connectedNodes);
  if (connectedSize == 0) {
    StackRemove(stack);
    return;
  }
  int toRemove = (connectedSize == 1) ? 0 : (rand() % connectedSize);
  GraphRemove(graph, node, connectedNodes[toRemove]);
  StackAdd(stack, connectedNodes[toRemove]);
}

int main() {
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(90);
  
  srand(time(NULL));

  Stack stack = StackNew(n * n);
  Graph graph = GraphNew(n * n);

  for (int i = 0; i < n * n; ++i) {
    if (i % n != 0) {
      GraphAdd(&graph, i - 1, i);
    }
    if (i % n != n - 1) {
      GraphAdd(&graph, i, i + 1);
    }
    if (i / n != 0) {
      GraphAdd(&graph, i - n, i);
    }
    if (i / n != n - 1) {
      GraphAdd(&graph, i, i + n);
    }
  }

  StackAdd(&stack, 0);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    mazeOnRedraw(&graph, &stack);
    reevalGraph(&graph, &stack);

    EndDrawing();
  }
  GraphDestroy(&graph);
  StackDestroy(&stack);
  CloseWindow();
  return 0;
}