#pragma once

#include <stack>
#include <array>
#include <string>

#include <SFML/Graphics.hpp>

#include "keyboard.h"

#define CARRY_FLAG 0xF
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 64

#define SOUND_SPEED 60
#define CPU_SPEED 500


class Chip8 {
    public:
        bool draw_flag;
        Keyboard keyboard;
        void perform_cycle();
        void load_font();
        void load_rom(std::string rom_name);
        void print_memory();
        void draw_screen(sf::RenderWindow& window);
        void update_timer(double delta);
        Chip8();

    private:
        // Fields
        uint16_t pc {0x200}; // Program Counter
        uint16_t I {0}; // Index Register
        uint8_t delay_timer {0};
        uint8_t sound_timer {60};
        double delay_timer_high_res {0};
        double sound_timer_high_res {60};
        std::array<uint8_t, 4096> memory {}; // 4K of memory
        std::array<uint8_t, 16> V {};  // 16 CPU registers. 15 general purpose registers and carry flag
        std::array<bool, SCREEN_HEIGHT * SCREEN_WIDTH> gfx {}; // GFX Buffer
        std::stack<uint16_t> stack;

        // Methods
        inline void increment_pc();
        uint16_t get_next_op_code();
        void set_delay_timer(uint8_t time);
        void set_sound_timer(uint8_t time);

        void handle_op_code(uint16_t op_code);
        void handle_op_code_0(uint16_t opcode);
        void handle_op_code_1(uint16_t opcode);
        void handle_op_code_2(uint16_t opcode);
        void handle_op_code_3(uint16_t opcode);
        void handle_op_code_4(uint16_t opcode);
        void handle_op_code_5(uint16_t opcode);
        void handle_op_code_6(uint16_t opcode);
        void handle_op_code_7(uint16_t opcode);
        void handle_op_code_8(uint16_t opcode);
        void handle_op_code_9(uint16_t opcode);
        void handle_op_code_A(uint16_t opcode);
        void handle_op_code_B(uint16_t opcode);
        void handle_op_code_C(uint16_t opcode);
        void handle_op_code_D(uint16_t opcode);
        void handle_op_code_E(uint16_t opcode);
        void handle_op_code_F(uint16_t opcode);
        void handle_op_code_unknown(uint16_t opcode);
};