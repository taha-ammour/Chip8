#ifndef CHIP8_H
#define CHIP8_H
#include "Config.h"
#include "Chip8_memory.h"
#include "Chip8_registers.h"
#include "Chip8_stack.h"
#include "Chip8_keyboard.h"
#include "Chip8_screen.h"

struct Chip8_t
{
    struct Chip8_memory_t memory;
    struct Chip8_stack_t stack;
    struct Chip8_registers_t registers;
    struct Chip8_keyboard_t keyboard;
    struct Chip8_screen_t screen;
};

void Chip8_init(struct Chip8_t * chip8);
#endif