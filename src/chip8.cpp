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

    uint16_t first_nibble = op_code & 0xF000;

    void (Chip8::*opcode_handler)(uint16_t op_code);

    switch(first_nibble) {
        case 0x0000:
            opcode_handler = &Chip8::handle_op_code_0;
            break;
        case 0x1000:
            opcode_handler = &Chip8::handle_op_code_1;
            break;
        case 0x2000:
            opcode_handler = &Chip8::handle_op_code_2;
            break;
        case 0x3000:
            opcode_handler = &Chip8::handle_op_code_3;
            break;
        case 0x4000:
            opcode_handler = &Chip8::handle_op_code_3;
            break;
        default:
            opcode_handler = &Chip8::handle_op_code_unknown;
            break;
    }

    (this->*opcode_handler)(op_code);

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

inline void Chip8::increment_pc() {
    pc += 2;
}

//  ---------- Opcode handlers ----------
void Chip8::handle_op_code_0(uint16_t opcode) {
    if (opcode == 0x00E0) {
        // Clear the screen by zeroing the gfx array
        for (int i=0; i<gfx.size(); i++) {
            gfx[i] = 0;
        }
        increment_pc();
    } else if (opcode == 0x00EE) {
        // Return from a subroutine by popping the stack
        pc = stack.top();
        stack.pop();
    }
}

void Chip8::handle_op_code_1(uint16_t opcode) {
    // Opcode 1NNN, GOTO to address NNN
    uint16_t address = opcode & 0x0FFF;
    pc = address;
}

void Chip8::handle_op_code_2(uint16_t opcode) {
    // Opcode 2NNN, Call subroutine at address NNN
    uint16_t address = opcode & 0x0FFF;
    stack.push(pc);
    pc = address;
}

void Chip8::handle_op_code_3(uint16_t opcode) {
    // Opcode 3XNN, Skip next instruction if Vx == NN
    uint16_t reg = (opcode & 0x0F00) >> 16;
    uint16_t val = opcode & 0x00FF;
    if (V[reg] == val) {
        increment_pc();
    }
    increment_pc();
}

void Chip8::handle_op_code_4(uint16_t opcode) {
    // Opcode 4XNN, Skip next instruction if Vx != NN
    uint16_t reg = (opcode & 0x0F00) >> 16;
    uint16_t val = opcode & 0x00FF;
    if (V[reg] != val) {
        increment_pc();
    }
    increment_pc();
}


void Chip8::handle_op_code_unknown(uint16_t opcode) {
    std::cerr << "Unknown opcode: " << std::hex << opcode << std::dec << std::endl;
    increment_pc(); // Does it really make sense to continue in the scenario?
}