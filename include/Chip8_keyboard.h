#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include <stdbool.h>
#include "Config.h"

struct Chip8_keyboard_t
{
    bool keyboard[CHIP8_TOTAL_KEYS];
};

int Chip8_keyboard_map(const char* map, char key);
void Chip8_keyboard_down(struct Chip8_keyboard_t* keyboard, int key);
void Chip8_keyboard_up(struct Chip8_keyboard_t* keyboard, int key);
bool Chip8_keyboard_isDown(struct Chip8_keyboard_t* keyboard, int key);

#endif