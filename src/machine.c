// Main working file
// Contains the definitions for ijvm.h functions

#include <stdlib.h>
#include "ijvm.h"
#include "util.h"
#include "parse.h"
#include "stack.h"
#include "frame.h"
#include "heap.h"
#include "network.h"
#include "instruction.h"

int init_ijvm(char *binary_file)                                // Loads binary and initializes ijvm instance
{
    FILE *fp;
    input = stdin;                                              // Setting default i/o
    output = stdout;

    word_t buffer = 0;
    fp = fopen(binary_file, "r");
    fread(&buffer, sizeof(word_t), 1, fp);                      // Reading magic number
    buffer = swap_uint32(buffer);

    if(buffer != MAGIC_NUMBER)                                  // Checking if correct M number to verify ijvm binary
    {
        return -1;
    }

    if(!parse_constants(fp) || !parse_text(fp))                 // Parsing constant and text blocks
    {
        return -1;                                              // Returns -1 on malloc fail
    }

    fclose(fp);                                                 // Binary parsed

    if(!init_stack() || !init_localFrame() || !init_heap())     // Initializing stack, local frame and heap
    {
        return -1;                                              // Returns -1 on malloc fail
    }

    return 0;
}

void destroy_ijvm()
{
    free(stackInfo.stack);                              // Deallocating stack array
    stackInfo.stack = NULL;
    free(instance.text);                                // Deallocating text array
    instance.text = NULL;
    free(instance.constants);                           // Deallocating constants array
    instance.constants = NULL;
    free_framelist(localFrame);                         // Deallocating local frame, as well other frames if program exited outside the primary frame
    destroy_heap();
}

bool step(void)
{
    if(instance.pc >= instance.textSize)
    {
        instance.pc = -1;                               // Assigns a "finished" value to pc - this is used in finished()
        return false;                                   // Program finishes execution successfully (hopefully)
    }
    //fprintf(stderr, "Instruction: %d, PC: %d, Stack size: %d, TOS: %d\n", get_instruction(), instance.pc, stack_size(), *stackInfo.stackPointer);
    if(!exec_instruction(instance.text[instance.pc]))   // Executes the next instruction from the text block, returns false on fail
    {
        fprintf(stderr, "--- FAILED ON PROGRAM COUNTER %d ---\n", instance.pc);
        return false;
    }

    instance.pc++;                                      // Increments to next instruction (pc is on the last arg after exec_instruction)
    return true;
}

void run()
{
    while(step());
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
    return instance.text;
}

int get_program_counter(void)
{
    return instance.pc;
}

byte_t get_instruction(void)
{
    return instance.text[instance.pc];
}

int text_size(void)
{
    return instance.textSize;
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
    return stackInfo.stackPointer - stackInfo.basePointer;
}

word_t get_local_variable(int i)
{
    return localFrame->locals[i];
}

word_t get_constant(int i)
{
    return instance.constants[i];
}

bool finished()
{
    if(instance.pc == -1)
    {
        return true;
    }
    return false;
}
