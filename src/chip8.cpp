#include <iostream>
#include <fstream>
#include "chip8.h"
#include "font.h"

void Chip8::load_font() {
    for (int i=0; i<CHIP8_FONT_SIZE; i++) {
        memory[i] = chip8_fontset[i];
    }
}

void Chip8::load_rom(std::string rom_name) {
    std::cout << pc << std::endl;
    std::ifstream file_stream;
    file_stream.open(rom_name, std::ios::in | std::ios::binary);  // TODO - Need to handle an error here.

    if (file_stream.is_open()) {
        file_stream.read((char *) &memory[pc], 4096 - pc);  // Assume the ROM will fit into memory
        file_stream.close();
    } else {
        // File doesn't exist
        // TODO - Throw something meaninful
        throw 2;
    }
}

uint16_t Chip8::get_next_op_code() {
    uint8_t first_byte = memory[pc];
    uint8_t second_byte = memory[pc + 1];
    return (first_byte << 8) | second_byte;
}

void Chip8::handle_op_code(uint16_t op_code) {
    std::cout << "Handling op code " << std::hex << op_code << ", PC: " << std::dec << pc << std::endl;
    pc += 2;
}

void Chip8::perform_cycle() {
    // Get the next op code from the rom
    // Parse the op code
    // Handle the opcode
    // Increment the PC if necessary
    uint16_t next_op_code = get_next_op_code();
    handle_op_code(next_op_code);
}

void Chip8::print_memory() {
    std::cout << "Memory dump: " << std::endl;
    for (int i=0; i < memory.size(); i++) {
        std::cout << std::dec << i << ": " << std::hex << (int)memory[i] << std::dec << std::endl;
    }
}