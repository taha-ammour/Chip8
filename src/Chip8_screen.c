#include "Chip8_screen.h"
#include <assert.h>


static void Chip8_screen_check_bounds(int x, int y){
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}
void Chip8_screen_set(struct Chip8_screen_t *screen, int x, int y)
{
    Chip8_screen_check_bounds(x,y);
    screen->pixels[y][x] = true;
}
bool Chip8_screen_isSet(struct Chip8_screen_t *screen, int x, int y)
{
    Chip8_screen_check_bounds(x,y);
    return screen->pixels[y][x];
}