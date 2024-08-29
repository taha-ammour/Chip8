#include "Chip8.h"
#include <memory.h>

void Chip8_init(struct Chip8_t * chip8){
    memset(chip8, 0, sizeof(struct Chip8_t));
}