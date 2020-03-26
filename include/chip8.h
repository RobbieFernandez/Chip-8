#include <stack>
#include <array>

class Chip8 {
    unsigned short pc {0x200}; // Program Counter
    unsigned short I {0}; // Index Register
    unsigned char delay_timer {0};
    unsigned char sound_timer {0};
    std::array<unsigned char, 4096> memory; // 4K of memory
    std::array<unsigned char, 16> V;  // 16 CPU registers. 15 general purpose registers and carry flag
    std::array<unsigned char, 64 * 32> gfx; // GFX Buffer
    std::stack<unsigned short> stack;

    public:
        void test();
};