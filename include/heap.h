// Heap functions
// Only part 6.1 of the bonus heap memory functionality was added (so no garbage collection)

#ifndef HEAP_H
#define HEAP_H
#include <stdlib.h>
#include "ijvm.h"

struct heapStruct
{
    word_t **heap;      // Main heap variable - contains all heap arrays
    int arrayNo;        // Stores current amount of arrays created
    int maxArrays;      // Maximum amount of arrays currently supported by **heap
    int *sizeArr;       // Contains the size of each array currently in the heap - e.g. array 0's size is sizeArr[0]
} heapInfo;

bool init_heap();       // Initializes heapInfo struct variable

bool newarray();        // Adds a new array to heap, potentially increasing its size

bool iaload();

bool iastore();

bool bounds_check();    // Check if any args of the load/store functions are outside the heap/heap array's bounds

void destroy_heap();    // Deallocates all memory assigned to the heap dynamic arrays

#endif
