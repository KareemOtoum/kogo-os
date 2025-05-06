#include "bitmap.h"

static uint8_t bitmap[phys::k_BitmapSize]{};

void* alloc_frame();
int get_free_bit(uint8_t pages);

void init_bitmap()
{
    kout << "setting used kernel pages\n";
    constexpr int total_pages = phys::k_BitmapSize * 8;

    for (int i = 0; i < total_pages; ++i)
    {
        if (frame_to_address(i) >= (void*)phys::k_KernelMemoryEnd)
        {
            kout << "Free pages start at page " << i << " address " << frame_to_address(i) << "\n";
            kout << "Pages allocated for OS: " << total_pages - (total_pages- i) << "\n";
            break;
        }

        // mark bit i as used
        int byte_index = i / 8;
        int bit_index = 7 - (i % 8);
        bitmap[byte_index] |= (1 << bit_index);
    }
}


int get_free_bit(uint8_t pages)
{
    for (int i = 0; i < 8; ++i)
    {
        if (!(pages & (1 << (7 - i)))) // check from MSB to LSB
        {
            return i; // return bit index 0 = leftmost bit
        }
    }

    return -1;
}

void* alloc_frame()
{
    for(int i{}; i < phys::k_BitmapSize; ++i)
    {
        int free_bit_index { get_free_bit(bitmap[i]) };
        if(free_bit_index != -1) // found a free page
        {
            int frame_num { i * 8 + free_bit_index };

            bitmap[i] |= (1 << (7 - free_bit_index));

            return frame_to_address(frame_num);
        }
    }

    return nullptr;
}

void free_frame(int i)
{
    int byte_index { i / 8 };
    if(byte_index < 0 || byte_index >= static_cast<int>(phys::k_BitmapSize)) 
    {
        kout << "\ninvalid free frame number\n";
        return;
    }

    bitmap[byte_index] |= 1 << (7 - i);
}