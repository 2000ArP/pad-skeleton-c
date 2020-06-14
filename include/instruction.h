#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "stack.h"
#include "util.h"

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

bool bipush()
{
    data.pc++;
    byte_t toPush = data.text[data.pc];
    return push((int8_t) toPush);
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

#endif
