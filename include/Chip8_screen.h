#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include <stdbool.h>
#include "Config.h"

struct Chip8_screen_t
{
    bool pixels[CHIP8_HEIGHT][CHIP8_WIDTH];
};

void Chip8_screen_set(struct Chip8_screen_t *screen, int x, int y);
bool Chip8_screen_isSet(struct Chip8_screen_t *screen, int x, int y);
#endif