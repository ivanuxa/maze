#include "raylib.h"
#include "graph.h"
#include "stack.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct {
  int m;
  int n;
  int screenWidth;
  int screenHeight;
  int cellSize;
  int mazeWidth;
  int mazeHeight;
  int offsetX;
  int offsetY;
  int drawType;
} Args;

void ArgsSetMazeSize(Args* args) {
  if (args->drawType == 1) {
    args->mazeWidth = args->m * args->cellSize;
    args->mazeHeight = args->n * args->cellSize;
  }
  else if (args->drawType == 2) {
    args->mazeWidth = (2 * args->m - 1) * args->cellSize;
    args->mazeHeight = (2 * args->n - 1) * args->cellSize;
  }
  args->offsetX = (args->screenWidth - args->mazeWidth) / 2;
  args->offsetY = (args->screenHeight - args->mazeHeight) / 2;
}


Args ArgsNew(int m, int n, int screenWidth, int screenHeight, int cellSize, int drawType) {
  Args args;
  args.m = m;
  args.n = n;
  args.screenWidth = screenWidth;
  args.screenHeight = screenHeight;
  args.cellSize = cellSize;
  args.drawType = drawType;
  ArgsSetMazeSize(&args);
  return args;
}


void mazeOnRedraw(Graph* graph, Stack* stack, Args* args) {
  int offsetX = args->offsetX;
  int offsetY = args->offsetY;
  int mazeHeight = args->mazeHeight;
  int mazeWidth = args->mazeWidth;
  int m = args->m;
  int n = args->n;
  int cellSize = args->cellSize;
  DrawLine(offsetX, offsetY, offsetX, offsetY + mazeHeight, BLACK);
  DrawLine(offsetX, offsetY, offsetX + mazeWidth, offsetY, BLACK);
  DrawLine(offsetX + mazeWidth + 1, offsetY, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  DrawLine(offsetX, offsetY + mazeHeight + 1, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  int nodeCount = graph->nodeCount;
  for (int i = 0; i < nodeCount; ++i) {
    int x = i % m;
    int y = i / m;
    DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, GraphNode(graph, i) ? RAYWHITE : BLUE);
    if (x != 0 && graph->edges[i - 1 + i * nodeCount]) {
      DrawLine(offsetX + x * cellSize, offsetY + y * cellSize, offsetX + x * cellSize, offsetY + y * cellSize + cellSize, BLACK);
    }
    if (x != m - 1 && graph->edges[i + 1 + i * nodeCount]) {
      DrawLine(offsetX + x * cellSize + cellSize, offsetY + y * cellSize, offsetX + x * cellSize + cellSize, offsetY + y * cellSize + cellSize, BLACK);
    }
    if (y != 0 && graph->edges[i - m + i * nodeCount]) {
      DrawLine(offsetX + x * cellSize, offsetY + y * cellSize, offsetX + x * cellSize + cellSize, offsetY + y * cellSize, BLACK);
    }
    if (y != n - 1 && graph->edges[i + m + i * nodeCount]) {
      DrawLine(offsetX + x * cellSize, offsetY + y * cellSize + cellSize, offsetX + x * cellSize + cellSize, offsetY + y * cellSize + cellSize, BLACK);
    }
  }
  if (stack->size > 0) {
    int node = StackPeek(stack);
    int x = node % m;
    int y = node / m;
    DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, RED);
  }
}

void mazeOnRedraw2(Graph* graph, Stack* stack, Args* args) {
  int offsetX = args->offsetX;
  int offsetY = args->offsetY;
  int mazeHeight = args->mazeHeight;
  int mazeWidth = args->mazeWidth;
  int m = args->m;
  int n = args->n;
  int cellSize = args->cellSize;
  const int actualM = 2 * m - 1;
  const int actualN = 2 * n - 1;
  DrawLine(offsetX, offsetY, offsetX, offsetY + mazeHeight, BLACK);
  DrawLine(offsetX, offsetY, offsetX + mazeWidth, offsetY, BLACK);
  DrawLine(offsetX + mazeWidth + 1, offsetY, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  DrawLine(offsetX, offsetY + mazeHeight + 1, offsetX + mazeWidth + 1, offsetY + mazeHeight + 1, BLACK);
  int nodeCount = graph->nodeCount;
  for (int i = 1; i < actualN; i += 2) {
    for (int j = 1; j < actualM; j += 2) {
      DrawRectangle(offsetX + j * cellSize, offsetY + i * cellSize, cellSize, cellSize, BLACK);
    }
  }
  for (int i = 0; i < nodeCount; ++i) {
    int x = (i % m) * 2;
    int y = (i / m) * 2;
    DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, GraphNode(graph, i) ? RAYWHITE : BLUE);
    if (x != 0 && graph->edges[i - 1 + i * nodeCount]) {
      DrawRectangle(offsetX + (x - 1) * cellSize, offsetY + y * cellSize, cellSize, cellSize, BLACK);
    }
    if (y != 0 && graph->edges[i - m + i * nodeCount]) {
      DrawRectangle(offsetX + x * cellSize, offsetY + (y - 1) * cellSize, cellSize, cellSize, BLACK);
    }
  }
  if (stack->size > 0) {
    int node = StackPeek(stack);
    int x = (node % m) * 2;
    int y = (node / m) * 2;
    DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, RED);
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

int main(int argc, char* argv[]) {

  // argv = %name% type cellSize m n

  Args args = ArgsNew(20, 15, 960, 960, 20, 1);
  if (argc == 2 && argv[1][0] == 'h') {
    printf("%s type cellSize m n", argv[0]);
    return 0;
  }
  if (argc == 4 || argc > 5 || (argc >= 2 && !(atoi(argv[1]) == 1 || atoi(argv[1]) == 2))) {
    return 1;
  }
  if (argc >= 2) {
    args.drawType = atoi(argv[1]);
    if (args.drawType == 2) {
      args.cellSize = 10;
    }
    printf("%d %d %d", args.drawType, args.mazeWidth, args.mazeHeight);
  }
  if (argc >= 3) {
    args.cellSize = atoi(argv[2]);
  }
  if (argc >= 5) {
    args.m = atoi(argv[3]);
    args.n = atoi(argv[4]);
  }
  ArgsSetMazeSize(&args);

  InitWindow(args.screenWidth, args.screenHeight, "raylib basic window");
  SetTargetFPS(60);
  
  srand(time(NULL));

  Stack stack = StackNew(args.m * args.n);
  Graph graph = GraphNew(args.m * args.n);

  for (int i = 0; i < args.m * args.n; ++i) {
    if (i % args.m != 0) {
      GraphAdd(&graph, i - 1, i);
    }
    if (i % args.m != args.m - 1) {
      GraphAdd(&graph, i, i + 1);
    }
    if (i / args.m != 0) {
      GraphAdd(&graph, i - args.m, i);
    }
    if (i / args.m != args.n - 1) {
      GraphAdd(&graph, i, i + args.m);
    }
  }

  StackAdd(&stack, rand() % (args.m * args.n));

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (args.drawType)
    {
    case 1:
      mazeOnRedraw(&graph, &stack, &args);
      break;
    case 2:
      mazeOnRedraw2(&graph, &stack, &args);
      break;
    }
    reevalGraph(&graph, &stack);

    EndDrawing();
  }
  GraphDestroy(&graph);
  StackDestroy(&stack);
  CloseWindow();
  return 0;
}