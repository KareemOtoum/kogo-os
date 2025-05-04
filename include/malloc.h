#pragma once
#include <cstdint>
#include <cstddef>
#include "kout.h"

namespace alloc
{
    extern "C" char _kernel_end;

    constexpr void* k_HeapStart { &_kernel_end };
    constexpr size_t k_HeapSize { 1024 * 1024 };
    constexpr int k_MagicNum { 1287419134 };
    constexpr char* k_HeapEnd { &_kernel_end + k_HeapSize };

    struct Header
    {
        size_t m_size{};
        int m_magic{};
    };

    struct Block
    {
        Block* m_next;
        size_t m_size;
    };
}

void create_heap();
size_t free_mem();
void log_freelist();
void scan_heap();

extern "C"
{
void* malloc(size_t size);
void free(void* ptr);
}