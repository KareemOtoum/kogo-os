
#pragma once

#include <cstdint>
#include "kout.h"

namespace phys
{
    // 4 KiB Pages
    constexpr int k_PageSize { 4096 };
    // 4 GiB Memory
    constexpr unsigned long long k_MaxMemory = 1ULL << 32;
    // 1 bit per page
    constexpr std::size_t k_BitmapSize { k_MaxMemory / k_PageSize / 8};
    constexpr uintptr_t k_KernelMemoryEnd = 0x00400000; // ~4 MB
}

void    init_bitmap();
void*   alloc_frame();
void    free_frame(int i);

constexpr void* frame_to_address(int i)
{
    return (void*)(i * phys::k_PageSize);
}

constexpr int address_to_frame(void* ptr)
{
    return reinterpret_cast<uintptr_t>(ptr) / phys::k_PageSize;
}