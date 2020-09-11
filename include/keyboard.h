#pragma once

#include <cstdint>
#include <array>

#include <SDL2/SDL.h>

class Keyboard {
    public:
        bool is_key_down(uint8_t key);
        bool is_key_up(uint8_t key);
        uint8_t await_key_press();
        void remap_key(uint8_t chip_key, SDL_Scancode);

    private:
        std::array<SDL_Scancode, 16> key_map {
            SDL_SCANCODE_X,
            SDL_SCANCODE_1,
            SDL_SCANCODE_2,
            SDL_SCANCODE_3,
            SDL_SCANCODE_Q,
            SDL_SCANCODE_W,
            SDL_SCANCODE_E,
            SDL_SCANCODE_A,
            SDL_SCANCODE_S,
            SDL_SCANCODE_D,
            SDL_SCANCODE_Z,
            SDL_SCANCODE_C,
            SDL_SCANCODE_4,
            SDL_SCANCODE_R,
            SDL_SCANCODE_F,
            SDL_SCANCODE_V,
        };
};