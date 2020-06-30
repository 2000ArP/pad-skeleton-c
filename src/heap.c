// Definitions for heap functions
// Only part 6.1 of the bonus heap memory functionality was added (so no garbage collection)

#include "heap.h"
#include "stack.h"

bool init_heap()                                            // Initializes heap stuff
{
    heapInfo.heap = malloc(sizeof(*heapInfo.heap));
    heapInfo.sizeArr = malloc(sizeof(int));
    if(heapInfo.heap == NULL || heapInfo.sizeArr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    heapInfo.arrayNo = 0;
    heapInfo.maxArrays = 1;
    return true;
}

bool newarray()
{
    word_t count = pop();                                   // Size of the new array, popped of the stack
    if(count < 0)
    {
        fprintf(stderr, "Negative count arg for newarray, halting...\n");
        return false;
    }

    heapInfo.arrayNo++;
    if(heapInfo.arrayNo > heapInfo.maxArrays)               // Extending space for new arrays
    {
        heapInfo.heap = realloc(heapInfo.heap, 2 * heapInfo.maxArrays * sizeof(*heapInfo.heap));
        heapInfo.sizeArr = realloc(heapInfo.sizeArr, 2 * heapInfo.maxArrays * sizeof(int));
        if(heapInfo.heap == NULL || heapInfo.sizeArr == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return false;
        }
        heapInfo.maxArrays *= 2;
    }

    heapInfo.heap[heapInfo.arrayNo - 1] = malloc(count * sizeof(word_t));   // New array gets initialized with size of count
    heapInfo.sizeArr[heapInfo.arrayNo - 1] = count;

    return push(heapInfo.arrayNo - 1);                                      // Array index gets pushed to stack, as array refrence
}

bool iaload()
{
    word_t arrRef = pop();                          // Specifies from which array to take the value from
    word_t index = pop();                           // Specifies which value to take
    if(!bounds_check(arrRef, index))
    {
        return false;
    }

    return push(heapInfo.heap[arrRef][index]);      // Pushes value to load onto stack
}

bool iastore()
{
    word_t arrRef = pop();
    word_t index = pop();
    word_t value = pop();
    if(!bounds_check(arrRef, index))
    {
        return false;
    }

    heapInfo.heap[arrRef][index] = value;
    return true;
}

bool bounds_check(word_t arrRef, word_t index)      // Checks if arguments are in bounds of the heap structure
{
    if(arrRef > heapInfo.arrayNo - 1)               // Checks array reference
    {
        fprintf(stderr, "iaload array reference out of bounds\n");
        return false;
    }
    if(index > heapInfo.sizeArr[arrRef] - 1)        // Checks index of variable in array
    {
        fprintf(stderr, "iaload index out of bounds, halting...\n");
        return false;
    }
    return true;
}

void destroy_heap()                                 // Frees heap arrays and heap structure
{
    if(heapInfo.heap != NULL)
    {
        for(int i = 0; i < heapInfo.arrayNo; i++)
        {
            free(heapInfo.heap[i]);
            heapInfo.heap[i] = NULL;
        }
        free(heapInfo.heap);
        heapInfo.heap = NULL;
    }
}
