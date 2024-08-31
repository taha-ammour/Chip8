#include "Chip8_stack.h"
#include "Chip8.h"
#include <assert.h>

static void Chip8_stack_in_Bounds(struct Chip8_t* chip8){
    assert(chip8->registers.SP < sizeof(chip8->stack.stack));
}
void Chip8_stack_push(struct Chip8_t* chip8, unsigned short val)
{
    chip8->registers.SP += 1;
    Chip8_stack_in_Bounds(chip8);

    chip8->stack.stack[chip8->registers.SP] = val;
}
unsigned short Chip8_stack_pop(struct Chip8_t* chip8)
{
    Chip8_stack_in_Bounds(chip8);
    unsigned short val = chip8->stack.stack[chip8->registers.SP];
    chip8->registers.SP -= 1;
    return val;

}