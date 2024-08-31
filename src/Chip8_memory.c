#include "Chip8_memory.h"
#include <assert.h>

static void Chip8_isMemory_in_Bounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}
void Chip8_memory_set(struct Chip8_memory_t *memory, int index, unsigned char val)
{
    Chip8_isMemory_in_Bounds(index);
    memory->memory[index] = val;
}
unsigned char Chip8_memory_get(struct Chip8_memory_t* memory,int index)
{
    Chip8_isMemory_in_Bounds(index);
    return memory->memory[index];
}
unsigned short Chip8_memory_get_short(struct Chip8_memory_t* memory,int index){
    unsigned char byte1 = Chip8_memory_get(memory, index);
    unsigned char byte2 = Chip8_memory_get(memory, index + 1);
    return (byte1 << 8) | byte2;
}