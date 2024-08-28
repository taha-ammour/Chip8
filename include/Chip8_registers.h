#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H
#include "Config.h"

struct Chip8_registers_t
{
    unsigned char V[CHIP8_TOTAL_DATA_REGISTERS]; 
    unsigned short I;// store memory addresses
    unsigned char DT;// delay_timer
    unsigned char ST;// sound_timer
    unsigned short PC;// program counter
    unsigned char SP;// stack pointer
};

#endif