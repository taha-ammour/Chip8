#include "Chip8_memory.h"
#include <assert.h>

static void Chip8_isMemory_in_Bounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}
void chip8_memory_set(struct Chip8_memory_t *memory, int index, unsigned char val)
{
    Chip8_isMemory_in_Bounds(index);
    memory->memory[index] = val;
}
unsigned char chip8_memory_get(struct Chip8_memory_t* memory,int index)
{
    Chip8_isMemory_in_Bounds(index);
    return memory->memory[index];
}