#include <ijvm.h>

static uint32_t swap_uint32(uint32_t num)
{
    return((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

int init_ijvm(char *binary_file)
{
    // Implement loading of binary here
    FILE *fp;
    word_t buffer = 0;
    fp = fopen(binary_file, "r");
    fread(&buffer, sizeof(word_t), 1, fp);      // Reading magic number
    buffer = swap_uint32(buffer);

    if(buffer != MAGIC_NUMBER)                  // Checking if correct M number to verify ijvm
    {
        return -1;
    }

    fread(&buffer, sizeof(word_t), 1, fp);      // Reading constant pool origin

    fread(&buffer, sizeof(word_t), 1, fp);      // Reading first block's size
    buffer = swap_uint32(buffer);
    int constantNo = buffer / 4;

    word_t constants[constantNo];
    // memset(constants, 0, sizeof(constants));    IDK if needed
    for(int i = 0; i < constantNo; i++)         // Parsing constant block
    {
        fread(&buffer, sizeof(word_t), 1, fp);
        buffer = swap_uint32(buffer);
        constants[i] = buffer;
    }

    fread(&buffer, sizeof(word_t), 1, fp);      // Reading text origin
    buffer = swap_uint32(buffer);
    word_t textOrigin = buffer;                 // IDK if this would be required later

    fread(&buffer, sizeof(word_t), 1, fp);      // Reading text block size
    buffer = swap_uint32(buffer);
    int textSize = buffer;

    byte_t iBuffer;
    for(int i = 0; i < 1; i++)           // Parsing text block
    {
        fread(&iBuffer, sizeof(byte_t), 1, fp);
        fprintf(stderr, "Instruction: %d\nBIPUSH: %d\n", iBuffer, OP_BIPUSH);
    }

    return 0;
}

void destroy_ijvm()
{
  // Reset IJVM state
}

void run()
{
  // Step while you can
}

void set_input(FILE *fp)
{
  // TODO: implement me
}

void set_output(FILE *fp)
{
  // TODO: implement me
}
