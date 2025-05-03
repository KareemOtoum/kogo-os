#pragma once

#include "idt.h"
#include <cstdint>

namespace kbrd
{
    inline void wait_input_ready() 
    {
        while (inb(0x64) & 0x02); // Wait until input buffer is clear
    }
    
    inline void wait_output_ready() 
    {
        while (!(inb(0x64) & 0x01)); // Wait until output buffer has data
    }    

    inline void ps2_write(uint8_t port, uint8_t value) 
    {
        wait_input_ready();
        outb(port, value);
    }
    
    inline uint8_t ps2_read() 
    {
        wait_output_ready();
        return inb(0x60);
    }

    enum scancode2_map : uint8_t
    {
        A = 0X1E,
        B = 0X30,
        C = 0X2E,
        D = 0X20,
        E = 0X12,
        F = 0X21,
        G = 0X22,
        H = 0X23,
        I = 0X17,
        J = 0X24,
        K = 0X25,
        L = 0X26,
        M = 0X32,
        N = 0X31,
        O = 0X18,
        P = 0X19,
        Q = 0X10,
        R = 0X13,
        S = 0X1F,
        T = 0X14,
        U = 0X16,
        V = 0X2F,
        W = 0X11,
        X = 0X2D,
        Y = 0X15,
        Z = 0X2C,
        ENTER_MAIN = 0X1C ,
        SPACE = 0X39
    };

    constexpr char translate(uint8_t scancode)
    {
        switch (scancode)
        {
            case A: return 'a';
            case B: return 'b';
            case C: return 'c';
            case D: return 'd';
            case E: return 'e';
            case F: return 'f';
            case G: return 'g';
            case H: return 'h';
            case I: return 'i';
            case J: return 'j';
            case K: return 'k';
            case L: return 'l';
            case M: return 'm';
            case N: return 'n';
            case O: return 'o';
            case P: return 'p';
            case Q: return 'q';
            case R: return 'r';
            case S: return 's';
            case T: return 't';
            case U: return 'u';
            case V: return 'v';
            case W: return 'w';
            case X: return 'x';
            case Y: return 'y';
            case Z: return 'z';
            case ENTER_MAIN: return '\n';
            case SPACE: return ' ';

            default: return '\0';
        }
    }
}