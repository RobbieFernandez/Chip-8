#include <algorithm>
#include "keyboard.h"

bool Keyboard::is_key_down(uint8_t key) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    return currentKeyStates[key_map[key]];
}

bool Keyboard::is_key_up(uint8_t key) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    return !currentKeyStates[key_map[key]];
}

uint8_t Keyboard::await_key_press() {
    auto check_key = [this](uint8_t k) {return is_key_down(k);};
    std::array<bool, 16> prev_key_state;
    std::transform (key_map.begin(), key_map.end(), prev_key_state.begin(), check_key);

    while (true) {
        std::array<bool, 16> key_state;
        std::transform (key_map.begin(), key_map.end(), key_state.begin(), check_key);

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