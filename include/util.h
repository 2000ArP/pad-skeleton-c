#ifndef UTIL_H
#define UTIL_H

struct binary
{
    word_t *constants, textOrigin;
    byte_t *text;
    int textSize;
    int pc;
    int constantsSize;
} data;

FILE *input;
FILE *output;

#include "stack.h"
#include "instruction.h"

/* debug print */
#ifdef DEBUG

#define dprintf(...) \
    fprintf(stderr,  __VA_ARGS__)
#else
#define dprintf(...)
#endif



static uint32_t swap_uint32(uint32_t num)
{
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

int instruction_type(byte_t ins)     // returns 0 if instruction has no args, 1 if it has 1 byte arg, 2 if 2 byte args, 3 if word arg, or 4 if not an instruction
{
    switch(ins)
    {
        case 0x10:
            return 1;
        case 0x59:
            return 0;
        case 0xFE:
            return 0;
        case 0xA7:
            return 3;
        case 0xFF:
            return 0;
        case 0x60:
            return 0;
        case 0x7E:
            return 0;
        case 0x99:
            return 3;
        case 0x9B:
            return 3;
        case 0x9F:
            return 3;
        case 0x84:
            return 2;
        case 0x15:
            return 1;
        case 0xFC:
            return 0;
        case 0xB6:
            return 3;
        case 0xB0:
            return 0;
        case 0xAC:
            return 0;
        case 0x36:
            return 1;
        case 0x64:
            return 0;
        case 0x13:
            return 3;
        case 0x00:
            return 0;
        case 0xFD:
            return 0;
        case 0x57:
            return 0;
        case 0x5F:
            return 0;
        case 0xC4:
            return 0;
        default:
            return 4;   // returned if this function's argument is not an opcode for an instruction
    }
}

bool exec_instruction(byte_t ins)       // Calls the ijvm instruction's respective method, returns false on failure
{
    switch(ins)
    {
        case 0x10:
            return bipush();
        case 0x59:
            return false;
        case 0xFE:
            return false;
        case 0xA7:
            return false;
        case 0xFF:
            return false;
        case 0x60:
            return iadd();
        case 0x7E:
            return iand();
        case 0x99:
            return false;
        case 0x9B:
            return false;
        case 0x9F:
            return false;
        case 0x84:
            return false;
        case 0x15:
            return false;
        case 0xFC:
            return in();
        case 0xB6:
            return false;
        case 0xB0:
            return ior();
        case 0xAC:
            return false;
        case 0x36:
            return false;
        case 0x64:
            return isub();
        case 0x13:
            return false;
        case 0x00:
            return false;
        case 0xFD:
            return false;
        case 0x57:
            return pop_ijvm();
        case 0x5F:
            return swap();
        case 0xC4:
            return false;
        default:
            return false;
    }
}

#endif
