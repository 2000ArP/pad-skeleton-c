// File containing any other utilitarian functions

#ifndef UTIL_H
#define UTIL_H
#include "ijvm.h"

/* debug print */
#ifdef DEBUG

#define dprintf(...) \
    fprintf(stderr,  __VA_ARGS__)
#else
#define dprintf(...)
#endif

uint32_t swap_uint32(uint32_t num);     // Swaps endianess of an int

#endif
