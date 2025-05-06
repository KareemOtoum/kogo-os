
#pragma once

#include <cstdint>
#include <array>

struct __attribute__((aligned(4096))) PageDirectory
{
    using Entry = uint32_t;

    std::array<Entry, 1024> entries{};
};

/* 
Bits  | Description
------|-------------
0     | Present (1 = mapped, 0 = not)
1     | Read/Write (1 = RW, 0 = read-only)
2     | User/Supervisor (1 = user, 0 = kernel)
3     | Write-Through Caching
4     | Cache Disable
5     | Accessed (set by CPU)
6     | Reserved
7     | Page Size (0 = 4 KB, 1 = 4 MB)
8-11  | Custom (OS-specific)
12-31 | Physical address of page table (aligned to 4 KB)
*/