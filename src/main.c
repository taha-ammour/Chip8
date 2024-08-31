#include <SDL2/SDL.h>
#include <stdio.h>
#include "Chip8.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int WinMain(int argc, char *args[])
{

    struct Chip8_t chip8;
    Chip8_init(&chip8);

    Chip8_screen_draw_sprite(&chip8.screen,0,0,&chip8.memory.memory[0x00],5);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(EMULATOR_WINDOW_TITTLE,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
                                          CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int quit = 0;

    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
            {
                char key = e.key.keysym.sym;
                int vkey = Chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    Chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;
            case SDL_KEYUP:
            {
                char key = e.key.keysym.sym;
                int vkey = Chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    Chip8_keyboard_up(&chip8.keyboard, vkey);
                }
            }
            break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (Chip8_screen_isSet(&chip8.screen,x,y))
                {
                    SDL_Rect r = {x * CHIP8_WINDOW_MULTIPLIER, y * CHIP8_WINDOW_MULTIPLIER, CHIP8_WINDOW_MULTIPLIER, CHIP8_WINDOW_MULTIPLIER};
                    SDL_RenderFillRect(renderer, &r);
                }
                
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
