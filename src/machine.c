#include "ijvm.h"
#include "util.h"
#include "stack.h"
#include "instruction.h"

int init_ijvm(char *binary_file)
{
    // Implement loading of binary here
    FILE *fp;
    input = stdin;                                  // Setting default i/o
    output = stdout;

    word_t buffer = 0;
    fp = fopen(binary_file, "r");
    fread(&buffer, sizeof(word_t), 1, fp);          // Reading magic number
    buffer = swap_uint32(buffer);

    if(buffer != MAGIC_NUMBER)                      // Checking if correct M number to verify ijvm
    {
        return -1;
    }

    fread(&buffer, sizeof(word_t), 1, fp);          // Reading constant pool origin

    fread(&buffer, sizeof(word_t), 1, fp);          // Reading first block's size
    buffer = swap_uint32(buffer);
    data.constantsSize = buffer / 4;

    data.constants = (word_t*) malloc(data.constantsSize * sizeof(word_t));
    if(data.constants == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return -1;
    }
    for(int i = 0; i < data.constantsSize; i++)             // Parsing constant block
    {
        fread(&buffer, sizeof(word_t), 1, fp);
        buffer = swap_uint32(buffer);
        data.constants[i] = buffer;
    }

    fread(&buffer, sizeof(word_t), 1, fp);          // Reading text origin
    buffer = swap_uint32(buffer);
    data.textOrigin = buffer;                   // IDK if this would be required later

    fread(&buffer, sizeof(word_t), 1, fp);          // Reading text block size
    buffer = swap_uint32(buffer);
    data.textSize = buffer;

    byte_t iBuffer;
    data.text = (byte_t*) malloc(data.textSize * sizeof(byte_t));
    if(data.text == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return -1;
    }
    for(int i = 0; i < data.textSize; i++)           // Parsing text block
    {
        fread(&iBuffer, sizeof(byte_t), 1, fp);
        data.text[i] = iBuffer;
    }
    data.pc = 0;                                // Binary parsed
    fclose(fp);

    stackInfo.stack = (word_t*) calloc(1, sizeof(word_t));
    if(stackInfo.stack == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return -1;
    }
    stackInfo.size = 0;
    stackInfo.maxSize = 1;
    stackInfo.stackPointer = stackInfo.stack;

    fprintf(stderr, "\n");
    return 0;
}

void destroy_ijvm()
{
    free(stackInfo.stack);
    free(data.text);
    free(data.constants);
    if(input != stdin)
    {
        fclose(input);
    }
    if(output != stdout)
    {
        fclose(output);
    }
}

bool step(void)
{
    if(data.pc >= data.textSize)
    {
        return false;
    }
    //fprintf(stderr, "Instruction %d: %d... ", data.pc, get_instruction());
    if(!exec_instruction(data.text[data.pc]))
    {
        //fprintf(stderr, "failed\n");
        //return false;
    }
    else
    {
        //fprintf(stderr, "done\n");
    }
    fprintf(stderr, "%d\n", top());
    data.pc++;
    return true;
}

void run()
{
    while(step())
    {
        ;
    }
}

void set_input(FILE *fp)
{
    input = fp;
}

void set_output(FILE *fp)
{
    output = fp;
}

byte_t *get_text(void)
{
    return data.text;
}

int get_program_counter(void)
{
    return data.pc;
}

byte_t get_instruction(void)
{
    return data.text[data.pc];
}

int text_size(void)
{
    return data.textSize;
}

word_t tos(void)
{
    return top();
}

word_t *get_stack(void)
{
    return stackInfo.stack;
}

int stack_size(void)
{
    return stackInfo.size;
}
