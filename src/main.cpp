#include <iostream>
#include "chip8.h"

#include <SFML/Graphics.hpp>

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

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "CHIP-8");

    sf::View view(
        sf::Vector2f((float) SCREEN_WIDTH / 2, (float) SCREEN_HEIGHT / 2),
        sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT)
    );

    window.setView(view);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                chip.draw_flag = true;
            }
        }
        chip.perform_cycle();
        if (chip.draw_flag) {
            chip.draw_screen(window);
        }
    }
}