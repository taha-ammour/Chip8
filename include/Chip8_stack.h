#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H
#include "Config.h"

struct Chip8_t;
struct Chip8_stack_t
{
    unsigned short stack[CHIP8_TOTAL_STACK_DEPTH];
};

void Chip8_stack_push(struct Chip8_t* chip8, unsigned short val);
unsigned short Chip8_stack_pop(struct Chip8_t* chip8);

#endif