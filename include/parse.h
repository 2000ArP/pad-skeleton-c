// File containing functions pertaining to parsing the binary, as well as getting arguments from text

#ifndef PARSE_H
#define PARSE_H
#include <stdlib.h>
#include "ijvm.h"

struct ijvm
{
    word_t *constants;
    int constantsSize;
    byte_t *text;
    int textSize;
    int pc;
    bool wide;
} instance;

bool parse_constants(FILE *fp);     // Reads the constants block from the binary

bool parse_text(FILE *fp);          // Reads the text block from the binary

short read_short_arg();             // Gets a short value from two bytes from text, based on current pc

unsigned short wide_read();         // Gets a short if previous instruction was wide, or a byte otherwise (from text)

#endif
