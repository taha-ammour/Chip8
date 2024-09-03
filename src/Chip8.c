#include "Chip8.h"
#include "SDL2/SDL.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


const char Chip8_default_character_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80};
void Chip8_init(struct Chip8_t *chip8)
{
    memset(chip8, 0, sizeof(struct Chip8_t));
    memcpy(&chip8->memory.memory, Chip8_default_character_set, sizeof(Chip8_default_character_set));
}
void Chip8_load(struct Chip8_t *chip8, const char *buf, size_t size)
{
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void Chip8_exec_extended_8(struct Chip8_t *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char Final_Nibble = opcode & 0x000f;
    unsigned short tmp = 0;

    switch (Final_Nibble)
    {
        // LD Vx, Vy, 8xy0 - STORES Vy in Vx , Vx = Vy
    case 0x00:
        chip8->registers.V[x] = chip8->registers.V[y];
        break;
        // OR Vx, Vy, 8xy1 - perform OR operation on Vx and Vy store it in Vx, Vx = Vx | Vy
    case 0x01:
        chip8->registers.V[x] |= chip8->registers.V[y];
        break;
        // AND Vx, Vy, 8xy2 - perform AND operation on Vx and Vy store it in Vx, Vx = Vx & Vy
    case 0x02:
        chip8->registers.V[x] &= chip8->registers.V[y];
        break;
        // XOR Vx, Vy, 8xy3 - perform XOR operation on Vx and Vy store it in Vx, Vx = Vx ^ Vy
    case 0x03:
        chip8->registers.V[x] ^= chip8->registers.V[y];
        break;
        // ADD Vx, Vy, 8xy4 - perform ADD operation on Vx and Vy, set VF to 1 if carry, 0 otherwise, Vx = Vx + Vy
    case 0x04:
        tmp = chip8->registers.V[x] + chip8->registers.V[y];
        chip8->registers.V[0x0f] = (tmp > 0xff) ? 1 : 0;
        chip8->registers.V[x] = tmp & 0xff;
        break;
        // SUB Vx, Vy, 8xy5 - perform subtraction of Vy from Vx, set VF = 1 if Vx > Vy, 0 otherwise, Vx = Vx - Vy
    case 0x05:
        chip8->registers.V[0x0f] = (chip8->registers.V[x] > chip8->registers.V[y]) ? 1 : 0;
        chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
        break;
        // SHR Vx {, Vy}, 8xy6 - Shift Vx right by 1. Set VF to the least significant bit of Vx before the shift.
    case 0x06:
        chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
        chip8->registers.V[x] >>= 1;
        break;
        // SUBN Vx, Vy, 8xy7 - Set Vx = Vy - Vx, set VF = 1 if Vy > Vx, 0 otherwise
    case 0x07:
        chip8->registers.V[0x0f] = (chip8->registers.V[y] > chip8->registers.V[x]) ? 1 : 0;
        chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;
        // SHL Vx {, Vy}, 8xyE - Shift Vx left by 1. Set VF to the most significant bit of Vx before the shift.
    case 0x0E:
        chip8->registers.V[0x0f] = (chip8->registers.V[x] & 0x80) >> 7;
        chip8->registers.V[x] <<= 1;
        break;
    }
}
static char Chip8_wait_for_key_press(struct Chip8_t *chip8)
{
    SDL_Event e;
    while (SDL_WaitEvent(&e))
    {
        if (e.type != SDL_KEYDOWN)
            continue;

        char c = e.key.keysym.sym;
        char chip8_key = Chip8_keyboard_map(&chip8->keyboard, c);
        if (chip8_key != -1)
        {
            return chip8_key;
        }
        
    }
    return -1;
    
}
static void Chip8_exec_extended_F(struct Chip8_t *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;

    switch (opcode & 0x00ff)
    {
        // Fx07 - LD Vx, DT: Set Vx = delay timer value
    case 0x07:
        chip8->registers.V[x] = chip8->registers.DT;
        break;
    
        // Fx0A - LD Vx, K: Wait for a key press, store the value in Vx
    case 0x0A:
        {
            char pressed_key = Chip8_wait_for_key_press(chip8);
            chip8->registers.V[x] = pressed_key;
        }
        break;
        // Fx15 - LD DT, Vx: Set delay timer = Vx
    case 0x15:
        chip8->registers.DT = chip8->registers.V[x];
        break;
        // Fx18 - LD ST, Vx: Set sound timer = Vx
    case 0x18:
        chip8->registers.ST = chip8->registers.V[x];
        break;
        // Fx1E - ADD I, Vx: Set I = I + Vx
    case 0x1E:
        chip8->registers.I += chip8->registers.V[x];
        break;
        // Fx29 - LD F, Vx: Set I = location of sprite for digit Vx
    case 0x29:
        chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT ;
        break;
        // Fx33 - LD [I], Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2
    case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x]/100;
            unsigned char dozens = (chip8->registers.V[x]/10)%10;
            unsigned char units = chip8->registers.V[x]%10;

            Chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
            Chip8_memory_set(&chip8->memory, chip8->registers.I+1, dozens);
            Chip8_memory_set(&chip8->memory, chip8->registers.I+2,  units);
        }
        break;
        // Fx55 - LD [I], Vx: Store registers V0 through Vx in memory starting at location I
    case 0x55:
    {
        for (int i = 0; i <= x; i++)
        {
            Chip8_memory_set(&chip8->memory, chip8->registers.I + i, chip8->registers.V[i]);
        }
    }
        break;
        // Fx65 - LD Vx, [I]: Read registers V0 through Vx from memory starting at location I
    case 0x65:
    {
        for (int i = 0; i <= x ; i++)
        {
            chip8->registers.V[i] = Chip8_memory_get(&chip8->memory, chip8->registers.I + i);
        }
    }
        break;
    }
}

/*
        nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
        n or nibble - A 4-bit value, the lowest 4 bits of the instruction
        x - A 4-bit value, the lower 4 bits of the high byte of the instruction
        y - A 4-bit value, the upper 4 bits of the low byte of the instruction
        kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/
static void Chip8_exec_extended(struct Chip8_t *chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned char n = opcode & 0x000f;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    switch (opcode & 0xf000)
    {
        // JP addr, 1nnn jump to lacation nnn
    case 0x1000:
        chip8->registers.PC = nnn;
        break;
        // Call adrr, Call subroutine at location nnn
    case 0x2000:
        Chip8_stack_push(chip8, chip8->registers.PC);
        chip8->registers.PC = nnn;
        break;
        // SE Vx, byte - 3xkk skip next instruction if Vx = kk
    case 0x3000:
        if (chip8->registers.V[x] == kk)
        {
            chip8->registers.PC += 2;
        }
        break;
        // SNE Vx, byte - 4xkk skip next instruction if Vx != kk
    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;
        // SE Vx, Vy - 5xy0, skip next instruction if Vx = Vy
    case 0x5000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;
        // LD Vx, byte - 0x6xkk load kk into Vx Vx = kk
    case 0x6000:
        chip8->registers.V[x] = kk;
        break;
        // ADD Vx, byte - 0x7xkk ADD kk to register Vx Vx += kk
    case 0x7000:
        chip8->registers.V[x] += kk;
        break;

    case 0x8000:
        Chip8_exec_extended_8(chip8, opcode);
        break;
        // SNE Vx, Vy - 9xy0, skip next instruction if Vx!= Vy
    case 0x9000:
        if (chip8->registers.V[x] != chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;
        // Annn - LD I, addr: Set I = nnn
    case 0xA000:
        chip8->registers.I = nnn;
        break;
        // Bnnn - JP V0, addr: Jump to location nnn + V0
    case 0xB000:
        chip8->registers.PC = nnn + chip8->registers.V[0x00];
        break;
        // Cxkk - RND Vx, byte: Set Vx = random byte AND kk
    case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 0x100) & kk;
        break;
        // Dxyn - DRW Vx, Vy, nibble: Draw sprite at (Vx, Vy) with n bytes of sprite data
    case 0xD000:
        {
            const char * sprite = (const char * ) &chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = Chip8_screen_draw_sprite(&chip8->screen,chip8->registers.V[x],chip8->registers.V[y],sprite, n);
        }
        break;
    case 0xE000:
        {
            switch (opcode & 0x00ff)
            {
                // EX9E - SKP Vx - Skip next instruction if key with the value of Vx is pressed
            case 0x9E:
                if(Chip8_keyboard_isDown(&chip8->keyboard,chip8->registers.V[x]))
                {
                    chip8->registers.PC += 2;
                }
                break;
            
                // EXA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not pressed
            case 0xA1:
                if(!Chip8_keyboard_isDown(&chip8->keyboard,chip8->registers.V[x]))
                {
                    chip8->registers.PC += 2;
                }
                break;
            }
        }
        break;

    case 0xF000:
            Chip8_exec_extended_F(chip8,opcode);
        break;
    }
}
void Chip8_exec(struct Chip8_t *chip8, unsigned short opcode)
{
    switch (opcode)
    {
    case 0x00E0: // CLS: Clearing display
        Chip8_screen_clear(&chip8->screen);
        break;
    case 0x00EE: // RET: return from subroutine
        chip8->registers.PC = Chip8_stack_pop(chip8);
        break;
    default:
        Chip8_exec_extended(chip8, opcode);
        break;
    }
}