#include "Chip8.h"
#include <memory.h>
#include <assert.h>

const char Chip8_default_character_set[] = {
    0xF0,0x90,0x90,0x90,0xF0,
    0x20,0x60,0x20,0x20,0x70,
    0xF0,0x10,0xF0,0x80,0xF0,
    0xF0,0x10,0xF0,0x10,0xF0,
    0x90,0x90,0xF0,0x10,0x10,
    0xF0,0x80,0xF0,0x10,0xF0,
    0xF0,0x80,0xF0,0x90,0xF0,
    0xF0,0x10,0x20,0x40,0x40,
    0xF0,0x90,0xF0,0x90,0xF0,
    0xF0,0x90,0xF0,0x10,0xF0,
    0xF0,0x90,0xF0,0x90,0x90,
    0xE0,0x90,0xE0,0x90,0xE0,
    0xF0,0x80,0x80,0x80,0xF0,
    0xE0,0x90,0x90,0x90,0xE0,
    0xF0,0x80,0xF0,0x80,0xF0,
    0xF0,0x80,0xF0,0x80,0x80
};
void Chip8_init(struct Chip8_t * chip8){
    memset(chip8, 0, sizeof(struct Chip8_t));
    memcpy(&chip8->memory.memory,Chip8_default_character_set, sizeof(Chip8_default_character_set));
}
void Chip8_load(struct Chip8_t * chip8, const char* buf, size_t size)
{
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS],buf,size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}   

/*
        nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
        n or nibble - A 4-bit value, the lowest 4 bits of the instruction
        x - A 4-bit value, the lower 4 bits of the high byte of the instruction
        y - A 4-bit value, the upper 4 bits of the low byte of the instruction
        kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/
static void Chip8_exec_extended(struct Chip8_t * chip8,unsigned short opcode)
{
 

}
void Chip8_exec(struct Chip8_t * chip8, unsigned short opcode)
{
    switch (opcode)
    {
    case 0x00E0://CLS: Clearing display
            Chip8_screen_clear(&chip8->screen);
        break;
    case 0x00EE://RET: return from subroutine
            chip8->registers.PC = Chip8_stack_pop(chip8);
        break;
    default: 
        Chip8_exec_extended(chip8, opcode);

    }
}