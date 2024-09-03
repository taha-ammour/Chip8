#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include <stdbool.h>
#include "Config.h"

struct Chip8_keyboard_t
{
    bool keyboard[CHIP8_TOTAL_KEYS];
    const char* keyboard_map;
};

void Chip8_keyboard_set_map(struct Chip8_keyboard_t* keyboard, const char* map);
int Chip8_keyboard_map(struct Chip8_keyboard_t* keyboard, char key);
void Chip8_keyboard_down(struct Chip8_keyboard_t* keyboard, int key);
void Chip8_keyboard_up(struct Chip8_keyboard_t* keyboard, int key);
bool Chip8_keyboard_isDown(struct Chip8_keyboard_t* keyboard, int key);

#endif