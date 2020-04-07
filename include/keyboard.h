#pragma once

#include <cstdint>
#include <array>

#include <SFML/Window.hpp>

class Keyboard {
    public:
        bool is_key_down(uint8_t key);
        bool is_key_up(uint8_t key);
        uint8_t await_key_press();
        void remap_key(uint8_t chip_key, sf::Keyboard::Key key);

    private:
        std::array<sf::Keyboard::Key, 16> key_map {
            sf::Keyboard::X,
            sf::Keyboard::Num1,
            sf::Keyboard::Num2,
            sf::Keyboard::Num3,
            sf::Keyboard::Q,
            sf::Keyboard::W,
            sf::Keyboard::E,
            sf::Keyboard::A,
            sf::Keyboard::S,
            sf::Keyboard::D,
            sf::Keyboard::Z,
            sf::Keyboard::C,
            sf::Keyboard::Num4,
            sf::Keyboard::R,
            sf::Keyboard::F,
            sf::Keyboard::V
        };
};