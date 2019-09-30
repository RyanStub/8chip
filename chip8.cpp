#include "chip8.h"
#include <array>
#include <iostream>

unsigned char fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};




Chip8::Chip8()
{
    initialize();
}




void Chip8::initialize()
{
    //
    pc = 0x200; 
    opcode = 0;
    I = 0;
    sPtr = 0;

    for(int i = 0; i < sizeof(stack)/sizeof(*stack); i++)
        stack[i] = 0;

    for(int i = 0; i < sizeof(memory)/sizeof(*memory); i++)
        memory[i] = 0;

    for(int i = 0; i < sizeof(V)/sizeof(*V); i++)
        V[i] = 0; 

    for(int i = 0; i < sizeof(screen)/sizeof(*screen); i++)
        screen[i] = 0; 

    for(int i = 0; i < 80; ++i)
        memory[i] = fontset[i];
}


//Initializes all attributes used in emmualtion
void Chip8::initialize()
{
    //
    pc = 0x200; 
    opcode = 0;
    I = 0;
    sPtr = 0;

    for(int i = 0; i < sizeof(stack)/sizeof(*stack); i++)
        stack[i] = 0;

    for(int i = 0; i < sizeof(memory)/sizeof(*memory); i++)
        memory[i] = 0;

    for(int i = 0; i < sizeof(V)/sizeof(*V); i++)
        V[i] = 0; 

    for(int i = 0; i < sizeof(screen)/sizeof(*screen); i++)
        screen[i] = 0; 

    for(int i = 0; i < 80; ++i)
        memory[i] = fontset[i];
}


//Emulates a single fetch, decode, excute cyvle of the cpu
void Chip8::cpuCylce()
{
    decode();
}


//Decodes and executes an instruction 
//Takes no parmaeter as the instance vaiable opcode is used
void Chip8::decode()
{
    //Identify the most significant 4 bits
    switch(opcode & 0xF000)
    {
        case 0x0000:
        
            switch (opcode & 0x0FFF)
            {
            //00E0: Clear screen
            case 0x00E0:
                for(int i = 0; i < sizeof(screen)/sizeof(*screen); i++)
                    screen[i] = 0; 
                pc += 2;
                break;
            
            //00EE: Returns from a subroutine.
            case 0x00EE:
                pc = stack[--sPtr];
                pc += 2;
                break;
           
            //0NNN: subroutein at 0x0NNN
            default:
                stack[sPtr++] = pc;
                pc = opcode & 0x0FFF;
                break;
            }
            break;
        //1NNN: Jumps to address NNN
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        

        //2NNN: Calls subroutine at NNN.
        case 0x2000:
            stack[sPtr++] = pc;
            pc = opcode & 0x0FFF;
            break;
        
        //3XNN: Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
        case 0x3000:
            if(V[(opcode & 0x0F00) >> 8] == 0x00FF)
                pc += 4;
            else
                pc += 2;
            break;
            
        
        //4XNN: Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
        case 0x4000:
            if(V[(opcode & 0x0F00) >> 8] != 0x00FF)
                pc += 4;
            else
                pc += 2;
            break;
        
        //5XY0: Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
        case 0x5000:
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
            break;
            
        //6XNN: Sets VX to NN.
        case 0x6000:
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        pc += 2;
        break;
            
        //7XNN: Adds NN to VX. (Carry flag is not changed)
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
            
        
        case 0x8000:
            switch(opcode & 0x000F)
            {
                //8XY0: Sets VX to the value of VY.
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] =  V[(opcode & 0x00F0) >> 4];
                    break;
                    
                //8XY1: Sets VX to VX or VY. (Bitwise OR operation)
                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |=  V[(opcode & 0x00F0) >> 4];
                    break;
            
                //8XY2: Sets VX to VX and VY. (Bitwise AND operation)
                case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &=  V[(opcode & 0x00F0) >> 4];
                    break;

                //8XY3: Sets VX to VX xor VY.
                case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
                    break;

                //8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                case 0x0004:
                    
                    V[0xF] = ((V[(opcode & 0x00F0) >> 4] + V[(opcode & 0x0F00) >> 8]) & 0x100) >> 8;
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] + V[(opcode & 0x0F00) >> 8]) & 0xFF;
                    pc += 2;
                    break;

                //8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                case 0x0005:
                    V[0xF] = (( V[(opcode & 0x0F00) >> 8] + (V[0xF] << 8) - V[(opcode & 0x00F0) >> 4]) & 0x100) >> 8;
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + (V[0xF] << 8) - V[(opcode & 0x00F0) >> 4]) & 0xFF;
                    pc += 2;
                    break;

                //8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
                case 0x0006:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1; 
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
                    pc += 2;

                //8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                case 0x0007:


                //8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
                case 0x0008:
            }
        
            
        
         case 0x9000:
        {
            
        }
         case 0xA000:
        {
            
        }
         case 0xB000:
        {
            
        }
         case 0xC000:
        {
            
        }
         case 0xD000:
        {
            
        }
         case 0xE000:
        {
            
        }
        case 0xF000:
        {
            
        }
         


        
    }


}



