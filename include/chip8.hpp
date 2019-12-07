#include <array>
#include <iostream>
class Chip8
{
//private instace variables 
private:
    
    //CHIP-8 has 35 opcodes, which are all two bytes long and stored big-endian
    unsigned short opcode;

    //CHIP-8 was most commonly implemented on 4K systems
    unsigned char memory[4096];

    //CHIP-8 has 16 8-bit data registers named V0 to VF.
    unsigned char V[16];

    //The address register, which is named I, is 16 bits wide and is used with several opcodes that involve memory operations.
    unsigned short I;
    unsigned short pc;
    
    //Original CHIP-8 Display resolution is 64×32 pixels, and color is monochrome
    bool screen[64 * 32];

    //CHIP-8 has two timers. They both count down at 60 hertz, until they reach 0.
    unsigned short DTimer;
    unsigned short STimer;

    // Stack: modern implementations normally have at least 16 levels.
    unsigned short stack[16];
    unsigned short sPtr;

    //Input is done with a hex keyboard that has 16 keys 
    unsigned char input[16];



public:
    Chip8();
    void initialize();
    void cpuCylce();


private:
    void fetch();
    void decode();


};