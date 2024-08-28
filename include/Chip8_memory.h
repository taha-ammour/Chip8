#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H
#include "Config.h"

struct Chip8_memory_t
{
    unsigned char memory[CHIP8_MEMORY_SIZE];
};

void chip8_memory_set(struct Chip8_memory_t* memory,int index, unsigned char val);
unsigned char chip8_memory_get(struct Chip8_memory_t* memory,int index);
#endif