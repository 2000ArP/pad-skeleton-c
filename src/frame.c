// File containing frame struct and functions directly affecting frames

#include "frame.h"
#include "parse.h"
#include "stack.h"

bool init_localFrame()
{
    localFrame = malloc(sizeof(struct frame));  // Initializing local frame
    if(localFrame == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    localFrame->locals = malloc(sizeof(word_t));
    if(localFrame->locals == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    localFrame->localsSize = 1;
    localFrame->prevNode = NULL;
    return true;
}

bool store_local(word_t newLocal, unsigned short index)         // Stores a new local, while incresing size of array if needed
{
    while(index + 1 > localFrame->localsSize)
    {
        localFrame->locals = realloc(localFrame->locals, 2 * localFrame->localsSize * sizeof(word_t));
        if(localFrame->locals == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return false;
        }
        localFrame->localsSize *= 2;
    }

    localFrame->locals[index] = newLocal;
    return true;
}

bool add_frame()
{
    struct frame *aux = localFrame;                             // Adds a new frame (new node) to the linked list
    localFrame = NULL;
    localFrame = malloc(sizeof(struct frame));
    if(localFrame == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    localFrame->locals = malloc(sizeof(word_t));
    if(localFrame->locals == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    localFrame->prevPc = instance.pc;                           // Saves details of the previous frame
    localFrame->prevBaseIndex = stackInfo.basePointer - stackInfo.stack;
    localFrame->prevNode = aux;

    return true;
}

void return_frame()                                             // Frees current frame and returns to the previous one
{
    free(localFrame->locals);
    struct frame *aux = localFrame->prevNode;
    free(localFrame);
    localFrame = aux;
}

void free_framelist(struct frame *currentFrame)                 // Recursively frees any remaining unreturned frames
{
    if(currentFrame->prevNode != NULL)
    {
        free_framelist(currentFrame->prevNode);
    }
    free(currentFrame->locals);
    free(currentFrame);
    currentFrame = NULL;
}
