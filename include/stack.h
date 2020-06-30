// File containing the stack structure and basic stack functions

#ifndef STACK_H
#define STACK_H
#include "ijvm.h"

struct stackStruct
{
    word_t *stack;              // Primary data structure to which the memory is allocated
    word_t *maxPointer;         // Points to the upper bound of the stack
    word_t *stackPointer;       // The top and base of stack of the current frame
    word_t *basePointer;
} stackInfo;

bool init_stack();              // Initializes stackInfo struct variable

bool push(word_t toPush);

word_t pop();

word_t top();

#endif
