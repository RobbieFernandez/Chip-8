#include <iostream>
#include <thread>
#include "chip8.h"

#define CPU_SPEED 500 // CPU Speed in Hz

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

    std::chrono::duration<double, std::milli> sleep_duration(1.0 / CPU_SPEED  * 1000);

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        chip.perform_cycle();
        chip.draw_screen();
        auto elapsed = start - std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(sleep_duration - elapsed);
    }
}