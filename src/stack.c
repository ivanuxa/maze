#include "stack.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"

void StackPrint(Stack* stack) {
    for (int i = 0; i < stack->size; ++i) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}

Stack StackNew(int capacity) {
    assert(capacity > 0);
    return (Stack) {
        malloc(capacity * sizeof(int)),
        0,
        capacity
    };
}
int StackPeek(Stack* stack) {
    assert(stack->size > 0);
    return stack->data[stack->size - 1];
}
int StackAdd(Stack* stack, int value) {
    if (stack->size == stack->capacity) {
        return 0;
    }
    stack->data[(stack->size)++] = value;
    return 1;
}
int StackRemove(Stack* stack) {
    if (stack->size == 0) {
        return 0;
    }
    (stack->size)--;
    return 1;
}

inline void StackDestroy(Stack* stack) {
    free(stack->data);
}
