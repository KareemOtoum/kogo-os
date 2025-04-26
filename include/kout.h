#pragma once

#include <cstddef>
#include <cstdint>

class KOut
{
public:
    KOut();

    KOut& operator<<(const char* str);
    KOut& operator<<(char c);
    KOut& operator<<(int num);
    KOut& operator<<(uint32_t num);
    KOut& operator<<(void* ptr);
};

extern KOut kout;

namespace terminal
{
    constexpr int k_VGAWidth { 80 };
    constexpr int k_VGAHeight { 25 };
    constexpr uint32_t k_VGAMemory { 0xB8000 };

    enum Color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };
    
    
    inline constexpr uint8_t vga_entry_color(Color fg, Color bg) 
    {
        return fg | bg << 4;
    }

    inline constexpr uint16_t vga_entry(unsigned char uc, uint8_t color) 
    {
        return (uint16_t) uc | (uint16_t) color << 8;
    }
}