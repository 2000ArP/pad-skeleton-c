// File containing all ijvm instructions, as well as the large switch used to decode opcodes

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "ijvm.h"

FILE *input;                                // Global FILE'S which represent the ijvm's i/o, used in the IN and OUT instructions
FILE *output;

bool exec_instruction(byte_t instruction);  // Large switch used to decode opcodes, defined below

bool halt();

bool iadd();

bool isub();

bool iand();

bool ior();

bool istore();

bool ldc_w();

bool bipush();

bool dup_ijvm();

bool err();

bool in();

bool invokevirtual();

bool ireturn();

bool out();

bool swap();

bool pop_ijvm();

bool goto_ijvm();

bool ifeq();

bool iflt();

bool if_icmpeq();

bool iinc();

bool iload();

bool wide();

#endif
