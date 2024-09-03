#include <SDL2/SDL.h>
#include <stdio.h>
#include "Chip8.h"

#ifdef _WIN32
#include <windows.h>


int main(int argc, char *argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    int argc;
    LPWSTR* argv_wide = CommandLineToArgvW(GetCommandLineW(), &argc);

    
    char** argv = (char**)malloc(argc * sizeof(char*));
    for (int i = 0; i < argc; i++) {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, argv_wide[i], -1, NULL, 0, NULL, NULL);
        argv[i] = (char*)malloc(size_needed);
        WideCharToMultiByte(CP_UTF8, 0, argv_wide[i], -1, argv[i], size_needed, NULL, NULL);
    }

    
    int result = main(argc, argv);

    
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
    LocalFree(argv_wide);

    
    return result;
}
#endif

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {
        printf("you must provide a file as a 2 arg !!!\n");
        return -1;
    }
    
    const char* filename = argv[1];
    printf("the file working on is: %s\n", filename);

    FILE* f = fopen(filename, "rb");
    if(!f)
    {
        printf("Unable to open file %s\n", filename);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f,0, SEEK_SET);

    char* buffer = (char*)malloc(size * sizeof(char));
    int res = fread(buffer, size, 1, f);
    
    if (res != 1)
    {
        printf("Failed to read from file \n");
        return -1;
    }

    struct Chip8_t chip8;
    Chip8_init(&chip8);
    Chip8_load(&chip8, buffer, size);
    Chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);

    free(buffer);
    fclose(f);
    
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
                int vkey = Chip8_keyboard_map(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    Chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;
            case SDL_KEYUP:
            {
                char key = e.key.keysym.sym;
                int vkey = Chip8_keyboard_map(&chip8.keyboard, key);
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

        if (chip8.registers.DT > 0)
        {
            SDL_Delay(10);
            chip8.registers.DT--;
        }

        if (chip8.registers.ST > 0)
        {
            _beep(13000,10 * chip8.registers.ST);
            chip8.registers.ST = 0;
        }
        
        unsigned short opcode = Chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        Chip8_exec(&chip8, opcode);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
