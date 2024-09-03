#include "Chip8_keyboard.h"
#include "assert.h"

static void Chip8_keyboard_ensure_in_Bounds(int key)
{
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

void Chip8_keyboard_set_map(struct Chip8_keyboard_t* keyboard, const char* map)
{
    keyboard->keyboard_map = map;
}

int Chip8_keyboard_map(struct Chip8_keyboard_t* keyboard, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (keyboard->keyboard_map[i] == key)
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