#include <iostream>
#include "chip8.h"

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

    while (true) {
        chip.perform_cycle();
    }
}