#include <iostream>
#include "chip8.h"

#include <SDL2/SDL.h>

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initialising SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return true;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "ROM path must be given as first argument." << std::endl;
        return 1;
    }

    std::string rom_name = argv[1];

    Chip8 chip;
    chip.load_font();

    try {
        chip.load_rom(rom_name);
    } catch(int err) {
        std::cerr << "File " << rom_name << " does not exist" << std::endl;
        return err;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    bool init_succeeded = init(window, renderer);

    if (!init_succeeded) {
        return 1;
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    bool user_quit = false;

    while (!user_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                user_quit = true;
            }
        }
        chip.perform_cycle();
        if (chip.draw_flag) {
            chip.draw_screen(renderer);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}