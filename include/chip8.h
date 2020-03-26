#include <stack>

class Chip8 {
    unsigned char memory[4096]; // 4K of memory
    unsigned char V[16];  // 16 CPU registers. 15 general purpose registers and carry flag
    unsigned short I; // Index Register
    unsigned short pc; // Program Counter
    unsigned char gfx[64 * 32]; // GFX Buffer
    unsigned char delay_timer;
    unsigned char sound_timer;
    std::stack<unsigned short> stack[16];

    public:
        void test();
};