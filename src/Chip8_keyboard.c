#include "Chip8_keyboard.h"
#include "assert.h"

static void Chip8_keyboard_ensure_in_Bounds(int key)
{
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

int Chip8_keyboard_map(const char *map, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (map[i] == key)
        {
            return i;
        }
        
    }
    return -1;
}
void Chip8_keyboard_down(struct Chip8_keyboard_t *keyboard, int key)
{
    keyboard->keyboard[key] = true;
}
void Chip8_keyboard_up(struct Chip8_keyboard_t *keyboard, int key)
{
    keyboard->keyboard[key] = false;
}
bool Chip8_keyboard_isDown(struct Chip8_keyboard_t *keyboard, int key)
{
    return keyboard->keyboard[key];
}