#include <iostream>
#include <random>
#include <fstream>
#include <thread>
#include <stdio.h>

#include <SFML/System/Vector2.hpp>

#include "chip8.h"
#include "font.h"

void Chip8::load_font() {
    for (int i=0; i<CHIP8_FONT_SIZE; i++) {
        memory[i] = chip8_fontset[i];
    }
}

void Chip8::load_rom(std::string rom_name) {
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
            opcode_handler = &Chip8::handle_op_code_4;
            break;
        case 0x5000:
            opcode_handler = &Chip8::handle_op_code_5;
            break;
        case 0x6000:
            opcode_handler = &Chip8::handle_op_code_6;
            break;
        case 0x7000:
            opcode_handler = &Chip8::handle_op_code_7;
            break;
        case 0x8000:
            opcode_handler = &Chip8::handle_op_code_8;
            break;
        case 0x9000:
            opcode_handler = &Chip8::handle_op_code_9;
            break;
        case 0xA000:
            opcode_handler = &Chip8::handle_op_code_A;
            break;
        case 0xB000:
            opcode_handler = &Chip8::handle_op_code_B;
            break;
        case 0xC000:
            opcode_handler = &Chip8::handle_op_code_C;
            break;
        case 0xD000:
            opcode_handler = &Chip8::handle_op_code_D;
            break;
        case 0xE000:
            opcode_handler = &Chip8::handle_op_code_E;
            break;
        case 0xF000:
            opcode_handler = &Chip8::handle_op_code_F;
            break;
        default:
            opcode_handler = &Chip8::handle_op_code_unknown;
            break;
    }

    (this->*opcode_handler)(op_code);
}

void Chip8::perform_cycle() {
    auto start = std::chrono::steady_clock::now();
    std::chrono::milliseconds sleep_duration((int) (1.0 / CPU_SPEED  * 1000));

    uint16_t next_op_code = get_next_op_code();
    handle_op_code(next_op_code);

    std::this_thread::sleep_for(sleep_duration );
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

    update_timer(elapsed_ms.count() * SOUND_SPEED);
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

void Chip8::update_timer(double delta) {
    delay_timer_high_res -= delta  / 1000;
    delay_timer_high_res = delay_timer_high_res < 0 ? 0 : delay_timer_high_res;
    delay_timer = (uint8_t) delay_timer_high_res;

    sound_timer_high_res -= delta / 1000;
    sound_timer_high_res = sound_timer_high_res < 0 ? 0 : sound_timer_high_res;
    sound_timer = (uint8_t) sound_timer_high_res;
}

void Chip8::set_sound_timer(uint8_t timer) {
    sound_timer_high_res = timer;
    sound_timer = timer;
}

void Chip8::set_delay_timer(uint8_t timer) {
    delay_timer_high_res = timer;
    delay_timer = timer;
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
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t val = opcode & 0x00FF;
    if (V[x] == val) {
        increment_pc();
    }
    increment_pc();
}

void Chip8::handle_op_code_4(uint16_t opcode) {
    // Opcode 4XNN, Skip next instruction if Vx != NN
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t val = opcode & 0x00FF;
    if (V[x] != val) {
        increment_pc();
    }
    increment_pc();
}

void Chip8::handle_op_code_5(uint16_t opcode) {
    // Opcode 5XY0, Skip next instruction if Vx == Vy
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] == V[y]) {
        increment_pc();
    }
    increment_pc();
}

void Chip8::handle_op_code_6(uint16_t opcode) {
    // Opcode 6XNN, Set Vx to NN
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t val = (opcode & 0x00FF);
    V[x] = val;
    increment_pc();
}

void Chip8::handle_op_code_7(uint16_t opcode) {
    // Opcode 7XNN, Adds NN to VX. (Carry flag is not changed)
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t val = (opcode & 0x00FF);
    V[x] += val;
    increment_pc();
}

void Chip8::handle_op_code_8(uint16_t opcode) {
    // Many opcodes begin with 8
    // They are all of the format 8XYN, Where X and Y refer to registers and N indicates the operation
    // Switch on N and perform the corresponding operation.
    uint8_t last_nibble = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (last_nibble) {
        case 0x0: {
            // 8XY0, Sets VX to the value of VY.
            V[x] = V[y];
            break;
        }
        case 0x1: {
            // 8XY1, Sets VX to VX or VY. (Bitwise OR operation)
            V[x] |= V[y];
            break;
        }
        case 0x2: {
            // 8XY2, Sets VX to VX and VY. (Bitwise AND operation)
            V[x] &= V[y];
            break;
        }
        case 0x3: {
            // 8XY3, Sets VX to VX xor VY.
            V[x] ^= V[y];
            break;
        }
        case 0x4: {
            // 8XY4, Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
            uint16_t res = V[x] + V[y];
            V[CARRY_FLAG] = res > 0xFF ? 1 : 0;
            res &= 0xFF; // Truncate to 8 bits
            V[x] = (uint8_t) res;
            break;
        }
        case 0x5: {
            // 8XY5, VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            V[CARRY_FLAG] = V[x] > V[y] ? 1 : 0;
            V[x] = V[x] - V[y];
            break;
        }
        case 0x6: {
            // 8XY6, Stores the least significant bit of VX in VF and then shifts VX to the right by 1.[b]
            V[CARRY_FLAG] = V[x] & 0x01;
            V[x] = V[x] >> 1;
            break;
        }
        case 0x7: {
            // 8XY7, Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            V[CARRY_FLAG] = V[y] > V[x] ? 1 : 0;
            V[x] = V[y] - V[x];
            break;
        }
        case 0xE: {
            // 8XYE, Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b]
            V[CARRY_FLAG] = (V[x] & 0x80) ? 1 : 0;
            V[x] = V[x] << 1;
            break;
        }
        default: {
            return handle_op_code_unknown(opcode);
        }
    }

    increment_pc();
}

void Chip8::handle_op_code_9(uint16_t opcode) {
    // Opcode 9XY0, Skips the next instruction if VX doesn't equal VY
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] != V[y]) {
        increment_pc();
    }
    increment_pc();
}

void Chip8::handle_op_code_A(uint16_t opcode) {
    // Opcode ANNN, Sets I to the address NNN.
    I = opcode & 0x0FFF;
    increment_pc();
}

void Chip8::handle_op_code_B(uint16_t opcode) {
    // Opcode BNNN, Jumps to the address NNN plus V0..
    uint16_t addr = opcode & 0x0FFF;
    pc = V[0] + addr;
}

void Chip8::handle_op_code_C(uint16_t opcode) {
    // Opcode CXNN, Sets VX to the result of a bitwise and operation on a random number and NN.
    std::random_device dev;  // TODO - Construct this only once
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
    uint8_t rand = dist(rng);

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t n = opcode & 0x00FF;
    V[x] = n & rand;
    increment_pc();
}

void Chip8::handle_op_code_D(uint16_t opcode) {
    // Opcode DXYN, Draws a sprite at coordinate (VX, VY)
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t height = (opcode & 0x000F);
    draw_flag = true;

    uint8_t xPos = V[x] % SCREEN_WIDTH;
    uint8_t yPos = V[y] % SCREEN_HEIGHT;
    uint8_t width = 8;
    uint8_t* pixel_row_ptr = &(memory[I]);

    bool pixels_changed = false;
    uint8_t max_row = std::min(yPos + height, SCREEN_HEIGHT);

    for (int row=yPos; row < max_row; row++) {
        uint8_t pixel_row = *pixel_row_ptr;
        pixel_row_ptr++;

        // Extract individual pixel values from this byte.
        std::array<bool, 8> pixel_values {
            (bool) (pixel_row & 0x80),
            (bool) (pixel_row & 0x40),
            (bool) (pixel_row & 0x20),
            (bool) (pixel_row & 0x10),
            (bool) (pixel_row & 0x8),
            (bool) (pixel_row & 0x4),
            (bool) (pixel_row & 0x2),
            (bool) (pixel_row & 0x1),
        };

        int gfx_array_offset = row * SCREEN_WIDTH;

        for (int i=0; i < width && xPos + i < SCREEN_WIDTH; i++) {
            int gfx_index = gfx_array_offset + xPos + i;
            bool pixel_value = pixel_values[i];
            bool old_gfx_value = gfx[gfx_index];
            gfx[gfx_index] = gfx[gfx_index] != pixel_value; // XOR
            pixels_changed = pixels_changed || (old_gfx_value && !gfx[gfx_index]);
        }
    }

    V[CARRY_FLAG] = pixels_changed ? 1 : 0;
    increment_pc();
}

void Chip8::handle_op_code_E(uint16_t opcode) {
    uint8_t last_byte = opcode & 0x00FF;
    if (last_byte == 0x9E) {
        // EX9E	Skips the next instruction if the key stored in VX is pressed.
        // TODO - Check this (Assume nothing's pressed for now)
    } else if (last_byte == 0xA1) {
        // EXA1	Skips the next instruction if the key stored in VX isn't pressed.
        // TODO - Check this (Assume nothing's pressed for now)
        increment_pc();
    }

    increment_pc();
}

void Chip8::handle_op_code_F(uint16_t opcode) {
    // Many opcodes begin with F
    // They are all of the format FXNN, Where X refers to a register and NN indicates the operation
    // Switch on NN and perform the corresponding operation.

    uint8_t last_byte = opcode & 0x00FF;
    uint8_t x = (opcode & 0x0F00) >> 8;

    switch(last_byte) {
        case 0x07: {
            // FX07 Sets VX to the value of the delay timer.
            V[x] = delay_timer;
            break;
        }
        case 0x0A: {
            // FX0A A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
            // TODO
            V[x] = 0x01;
            break;
        }
        case 0x15: {
            // FX15 Sets the delay timer to VX.
            set_delay_timer(V[x]);
            break;
        }
        case 0x18: {
            // FX18 Sets the sound timer to VX.
            set_sound_timer(V[x]);
            break;
        }
        case 0x1E: {
            // FX1E Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
            uint16_t val = I + V[x];
            V[CARRY_FLAG] = val > 0xFFF ? 1 : 0;
            I = val & 0xFFF;
            break;
        }
        case 0x29: {
            // FX29 Sets I to the location of the sprite for the character in VX.
            uint8_t character = V[x];
            I = character * 5;
            break;
        }
        case 0x33: {
            // FX33 Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I,
            // the middle digit at I plus 1, and the least significant digit at I plus 2.
            uint8_t val = V[x];
            memory[I] = val / 100; // hundreds
            memory[I + 1] = (val % 100) / 10; // tens;
            memory[I + 2] = val % 10; // ones
            break;
        }
        case 0x55: {
            // FX55 Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
            for (int i=0; i<=x; i++) {
                memory[I + i] = V[i];
            }
            break;
        }
        case 0x65: {
            // FX65 Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
            for (int i=0; i<=x; i++) {
                V[i] = memory[I + i];
            }
            break;
        }
        default: {
            return handle_op_code_unknown(opcode);
        }
    }

    increment_pc();
}


void Chip8::handle_op_code_unknown(uint16_t opcode) {
    std::cerr << "Unknown opcode: " << std::hex << opcode << std::dec << std::endl;
    increment_pc(); // Does it really make sense to continue in the scenario?
}

void Chip8::draw_screen(sf::RenderWindow& window) {
    draw_flag = false;
    window.clear(sf::Color::Black);

    sf::RectangleShape pixel(sf::Vector2f(1, 1));

    for (int row=0; row < SCREEN_HEIGHT; row++) {
        for (int col=0; col < SCREEN_WIDTH; col++) {
            int i = row * SCREEN_WIDTH + col;
            if (gfx[i]) {
                pixel.setPosition(col, row);
                window.draw(pixel);
            }
        }
    }

    window.display();
}