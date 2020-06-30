// File containing the stack structure and basic stack functions
#include <stdlib.h>
#include "stack.h"

bool init_stack()                                           // Initializing stack
{
    stackInfo.stack = (word_t*) calloc(1, sizeof(word_t));
    if(stackInfo.stack == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    stackInfo.basePointer = stackInfo.stack;
    stackInfo.stackPointer = stackInfo.stack;
    stackInfo.maxPointer = stackInfo.stack + 1;
    return true;
}

bool push(word_t toPush)              // Returns -1 on failure
{
    if(stackInfo.stackPointer >= stackInfo.maxPointer - 1)                  // Increasing size of stack
    {
        int stackPointerIndex = stackInfo.stackPointer - stackInfo.stack;
        int basePointerIndex = stackInfo.basePointer - stackInfo.stack;
        int size = stackInfo.maxPointer - stackInfo.stack;
        stackInfo.stack = realloc(stackInfo.stack, 2 * size * sizeof(word_t));
        if(stackInfo.stack == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return false;
        }
        size *= 2;

        stackInfo.stackPointer = &stackInfo.stack[stackPointerIndex];   // stack could have changed address, so pointers are reassigned
        stackInfo.basePointer = &stackInfo.stack[basePointerIndex];
        stackInfo.maxPointer = &stackInfo.stack[size];
    }

    stackInfo.stackPointer++;                                           // Pushing new value atop the stack
    *stackInfo.stackPointer = toPush;

    return true;
}

word_t pop()
{
    if(stackInfo.stackPointer <= stackInfo.basePointer)
    {
        fprintf(stderr, "Warning: pop() failed, stack empty\n");
        return -1;
    }

    word_t top = *stackInfo.stackPointer;
    stackInfo.stackPointer--;
    return top;
}

word_t top()
{
    if(stackInfo.stackPointer <= stackInfo.basePointer)
    {
        fprintf(stderr, "Warning: top() failed, stack empty\n");
        return -1;
    }

    return *stackInfo.stackPointer;
}
