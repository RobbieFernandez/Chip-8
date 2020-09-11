#include "keyboard.h"

bool Keyboard::is_key_down(uint8_t key) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    return currentKeyStates[key_map[key]];
}

bool Keyboard::is_key_up(uint8_t key) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    return !currentKeyStates[key_map[key]];
}

std::array<bool, 16> Keyboard::get_key_state() {
    std::array<bool, 16> key_state;

    for (int i=0; i<16; i++) {
        key_state[i] = is_key_down(i);
    }

    return key_state;
}

uint8_t Keyboard::await_key_press() {
    auto prev_key_state = get_key_state();

    while (true) {
        auto key_state = get_key_state();

        for (int k=0; k<=0xF; k++) {
            if(key_state[k] && !prev_key_state[k]) {
                return k;
            }
        }

        prev_key_state = key_state;
    }
}

void Keyboard::remap_key(uint8_t chip_key, SDL_Scancode key) {
    key_map[chip_key] = key;
}