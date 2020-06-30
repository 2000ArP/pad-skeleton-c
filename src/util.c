// File containing any other utilitarian functions
#include "util.h"

uint32_t swap_uint32(uint32_t num)       // Swaps endianess
{
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}
