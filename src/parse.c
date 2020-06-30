// File containing functions pertaining to parsing the binary, as well as getting arguments from text
#include "parse.h"
#include "util.h"

bool parse_constants(FILE *fp)
{
    word_t buffer = 0;
    fread(&buffer, sizeof(word_t), 1, fp);                      // Reading constant pool origin

    fread(&buffer, sizeof(word_t), 1, fp);                      // Reading constant block's size
    buffer = swap_uint32(buffer);
    instance.constantsSize = buffer / 4;

    instance.constants = malloc(instance.constantsSize * sizeof(word_t));
    if(instance.constants == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    for(int i = 0; i < instance.constantsSize; i++)                 // Parsing constant block
    {
        fread(&buffer, sizeof(word_t), 1, fp);
        buffer = swap_uint32(buffer);
        instance.constants[i] = buffer;
    }

    return true;
}

bool parse_text(FILE *fp)
{
    word_t buffer = 0;
    fread(&buffer, sizeof(word_t), 1, fp);                      // Reading text origin
    buffer = swap_uint32(buffer);

    fread(&buffer, sizeof(word_t), 1, fp);                      // Reading text block size
    buffer = swap_uint32(buffer);
    instance.textSize = buffer;

    byte_t byteBuffer;
    instance.text = malloc(instance.textSize * sizeof(byte_t));
    if(instance.text == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    for(int i = 0; i < instance.textSize; i++)                  // Parsing text block
    {
        fread(&byteBuffer, sizeof(byte_t), 1, fp);
        instance.text[i] = byteBuffer;
        //fprintf(stderr, "%d: %d\n", i, byteBuffer);
    }
    instance.wide = false;
    instance.pc = 0;

    return true;
}

short read_short_arg()                                          // Reads a 2 byte method argument from the text block
{
    byte_t bytes[2];
    bytes[0] = instance.text[instance.pc];
    bytes[1] = instance.text[instance.pc + 1];
    short res = ((bytes[0] & 0xFF) << 8) | (bytes[1] & 0xFF);   // Converts 2 byte array to a short var
    return res;
}

unsigned short wide_read()
{
    if(instance.wide)                           // Reads index of local (2 bytes if wide, or 1 byte otherwise)
    {
        instance.pc++;
        unsigned short ret = (unsigned short) read_short_arg();
        instance.pc++;
        instance.wide = false;
        return ret;
    }
    else
    {
        instance.pc++;
        return instance.text[instance.pc];
    }
}
