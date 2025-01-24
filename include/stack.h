#pragma once
typedef struct {
    int* data;
    int size;
    int capacity;
} Stack;

Stack StackNew(int capacity);
int StackPeek(Stack* stack);
int StackAdd(Stack* stack, int value);
int StackRemove(Stack* stack);
void StackDestroy(Stack* stack); // ?
void StackPrint(Stack* stack);
