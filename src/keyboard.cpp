#include "keyboard.h"

bool Keyboard::is_key_down(uint8_t key) {
    return sf::Keyboard::isKeyPressed(key_map[key]);
}

bool Keyboard::is_key_up(uint8_t key) {
    return !is_key_down(key);
}

uint8_t Keyboard::await_key_press() {
    // TODO - Use the SFML window events to wait for this
    return 0x0;
}

void Keyboard::remap_key(uint8_t chip_key, sf::Keyboard::Key key) {
    key_map[chip_key] = key;
}