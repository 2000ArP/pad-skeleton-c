#ifndef STACK_H
#define STACK_H

struct stackStruct
{
    word_t *stack;
    int size;
    int maxSize;
    word_t *stackPointer;
} stackInfo;

bool push(word_t toPush)              // Returns -1 on failure
{
    if(stackInfo.size + 1 > stackInfo.maxSize)
    {
        stackInfo.stack = realloc(stackInfo.stack, 2 * stackInfo.maxSize * sizeof(word_t));
        stackInfo.maxSize *= 2;

        if(stackInfo.stack == NULL)
        {
            fprintf(stderr, "Memory allocation failed");
            return false;
        }
    }
    stackInfo.size++;
    stackInfo.stackPointer++;
    *stackInfo.stackPointer = toPush;

    return true;
}

word_t pop()
{
    if(stackInfo.size <= 0)
    {
        fprintf(stderr, "Warning: pop() failed, stack empty");
        return 0;
    }

    word_t top = *stackInfo.stackPointer;
    stackInfo.size--;
    stackInfo.stackPointer--;

    return top;
}

word_t top()
{
    if(stackInfo.size <= 0)
    {
        fprintf(stderr, "Warning: top() failed, stack empty");
        return 0;
    }

    return *stackInfo.stackPointer;
}

#endif
