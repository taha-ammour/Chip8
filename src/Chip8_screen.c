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

bool Chip8_screen_draw_sprite(struct Chip8_screen_t *screen, int x, int y, const char *sprite, int num)
{
    bool pixelCollision = false;

    for (int ly = 0; ly < num; ly++)
    {
        char c = sprite[ly];
        for (int lx = 0; lx < 8; lx++)
        {
            if((c & (0b10000000 >> lx)) == 0)
                continue;
            
            if (screen->pixels[(ly+y)%CHIP8_HEIGHT][(lx+x)%CHIP8_WIDTH])
                pixelCollision = true;
            
            screen->pixels[(ly+y)%CHIP8_HEIGHT][(lx+x)%CHIP8_WIDTH] ^= true;
        }
        
    }
    
    return pixelCollision;
}
