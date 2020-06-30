// File containing frame struct and functions directly affecting frames

#ifndef FRAME_H
#define FRAME_H
#include <stdlib.h>
#include "ijvm.h"

struct frame                    // Data structure containing the local vars, as well as acting as a node of a linked-list of frames
{
    word_t *locals;             // Contains all local vars of the current frame
    int localsSize;
    struct frame *prevNode;     // Stores previous pc, bp, and frame struct
    int prevPc;                 // PC position of the previous frame
    int prevBaseIndex;          // BP position of the previous frame
} *localFrame;                  // localFrame is treated as the top of the list / current frame


bool init_localFrame();         // Initializes localFrame struct

bool store_local(word_t newLocal, unsigned short index);        // Stores a local var at index, potentially increasing local array size

bool add_frame();               // Adds a new frame structure to the linked list of frames, saves details of previous frames

void return_frame();            // Called on the return of an ijvm method, removes the current frame, returning to the previous one

void free_framelist(struct frame *currentFrame);                // Deallocates any memory assigned to remaining frames

#endif
