// File containing all ijvm instructions, as well as the large switch used to decode opcodes
#include <stdlib.h>
#include "instruction.h"
#include "stack.h"
#include "parse.h"
#include "frame.h"
#include "heap.h"
#include "network.h"

FILE *input;                                // Global FILE'S which represent the ijvm's i/o, used in the IN and OUT instructions
FILE *output;

bool halt()
{
    instance.pc = instance.textSize;                // Causes step() to return false
    return true;
}

bool iadd()
{
    word_t add1 = pop();
    word_t sum = pop() + add1;
    return push(sum);
}

bool isub()
{
    word_t sub = pop();
    word_t min = pop();
    word_t dif = min - sub;
    return push(dif);
}

bool iand()
{
    word_t and1 = pop();
    word_t res = and1 & pop();
    return push(res);
}

bool ior()
{
    word_t or1 = pop();
    word_t res = or1 | pop();
    return push(res);
}

bool istore()
{
    word_t newLocal = pop();
    unsigned short index = wide_read();

    return store_local(newLocal, index);        // Method to assign var to locals array, potentially realloc-ing memory
}

bool ldc_w()
{
    instance.pc++;
    unsigned short index = (unsigned short) read_short_arg();
    instance.pc++;
    return push(instance.constants[index]);
}

bool bipush()
{
    instance.pc++;
    byte_t toPush = instance.text[instance.pc];
    return push((int8_t) toPush);               // Cast to int_8 to avoid sign problems
}

bool dup_ijvm()
{
    return push(top());
}

bool err()
{
    fprintf(output, "Error");
    return false;
}

bool in()
{
    word_t toPush = (word_t) getc(input);
    if(toPush == EOF)
    {
        return push(0);
    }
    else
    {
        return push(toPush);
    }
}

bool invokevirtual()
{
    instance.pc++;
    int index = (unsigned short) read_short_arg();      // Reading index for method address
    instance.pc++;
    if(!add_frame())                                    // Creates a new frame for the invoked method (also saves the current frame, so it can return to it)
    {
        return false;
    }
    instance.pc = instance.constants[index];            // Sets the pc to the invoked function

    int argsNum = (unsigned short) read_short_arg();    // Reading the number of args and the size of the local area, their sum is the total size of the locals array
    instance.pc += 2;
    int newLocalsSize = argsNum + (unsigned short) read_short_arg();
    instance.pc++;

    localFrame->locals = (word_t*) realloc(localFrame->locals, newLocalsSize * sizeof(word_t));
    if(localFrame->locals == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    localFrame->localsSize = newLocalsSize;

    for(int i = argsNum - 1; i >= 0; i--)               // Popping the method arguments and loading them in the locals array
    {
        localFrame->locals[i] = pop();
    }

    stackInfo.basePointer = stackInfo.stackPointer;     // Resetting the base pointer for the new frame

    return true;
}

bool ireturn()      // Restores the previous frame's data and returns the word on top of the stack
{
    instance.pc = localFrame->prevPc;
    word_t ret = *stackInfo.stackPointer;
    stackInfo.stackPointer = stackInfo.basePointer;
    stackInfo.basePointer = &stackInfo.stack[localFrame->prevBaseIndex];
    return_frame();
    return push(ret);
}

bool out()
{
    fprintf(output, "%c", (char) pop());
    return true;
}

bool swap()
{
    word_t var1 = pop();
    word_t var2 = pop();
    if(push(var1) == false)
    {
        return false;
    }
    return push(var2);
}

bool pop_ijvm()
{
    pop();
    return true;
}

bool goto_ijvm()
{
    instance.pc++;
    short offset = read_short_arg();
    instance.pc = instance.pc - 2 + offset;
    return true;
}

bool ifeq()
{
    if(pop() == 0)
    {
        return goto_ijvm();
    }
    instance.pc += 2;
    return true;
}

bool iflt()
{
    if(pop() < 0)
    {
        return goto_ijvm();
    }
    instance.pc += 2;
    return true;
}

bool if_icmpeq()
{
    word_t word1 = pop();
    if(word1 == pop())
    {
        return goto_ijvm();
    }
    instance.pc += 2;
    return true;
}

bool iinc()
{
    unsigned short index = wide_read();                 // Getting index

    instance.pc++;
    byte_t toAdd = instance.text[instance.pc];          // Getting value to add

    localFrame->locals[index] += ((int8_t) toAdd);      // Cast to int_8 to avoid sign problems
    return true;
}

bool iload()
{
    unsigned short index = wide_read();
    return push(localFrame->locals[index]);
}

bool wide()
{
    instance.wide = true;
    instance.pc++;
    return exec_instruction(instance.text[instance.pc]);    // Executes the instruction using the wide index
}

bool exec_instruction(byte_t instruction)       // Calls the ijvm instruction's respective method, returns false on failure
{
    switch(instruction)
    {
        case OP_BIPUSH:
            return bipush();
        case OP_DUP:
            return dup_ijvm();
        case OP_ERR:
            return err();
        case OP_GOTO:
            return goto_ijvm();
        case OP_HALT:
            return halt();
        case OP_IADD:
            return iadd();
        case OP_IAND:
            return iand();
        case OP_IFEQ:
            return ifeq();
        case OP_IFLT:
            return iflt();
        case OP_ICMPEQ:
            return if_icmpeq();
        case OP_IINC:
            return iinc();
        case OP_ILOAD:
            return iload();
        case OP_IN:
            return in();
        case OP_INVOKEVIRTUAL:
            return invokevirtual();
        case OP_IOR:
            return ior();
        case OP_IRETURN:
            return ireturn();
        case OP_ISTORE:
            return istore();
        case OP_ISUB:
            return isub();
        case OP_LDC_W:
            return ldc_w();
        case OP_NOP:
            return true;
        case OP_OUT:
            return out();
        case OP_POP:
            return pop_ijvm();
        case OP_SWAP:
            return swap();
        case OP_WIDE:
            return wide();
        case 0xD1:
            return newarray();
        case 0xD2:
            return iaload();
        case 0xD3:
            return iastore();
        case 0xE1:
            return netbind();
        case 0xE2:
            return netconnect();
        case 0xE3:
            return netin();
        case 0xE4:
            return netout();
        case 0xE5:
            return netclose();
        default:
            return false;
    }
}
