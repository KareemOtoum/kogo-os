
#pragma once

#include <cstdint>
#include <array>

struct __attribute__((aligned(4096))) PageTable
{
    using Entry = uint32_t;

    std::array<Entry, 1024> entries{};
};

/*
Bits  | Description
------|-------------
0     | Present
1     | Read/Write
2     | User/Supervisor
3     | Write-Through
4     | Cache Disable
5     | Accessed
6     | Dirty (set if page is written to)
7     | PAT (Page Attribute Table)
8-11  | Custom (OS-specific)
12-31 | Physical address of the 4 KB frame
*/