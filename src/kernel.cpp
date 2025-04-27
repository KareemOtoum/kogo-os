#include "kout.h"
#include "malloc.h"
#include "init.h"

void malloc_test();
void print_logo();

extern "C" void kernel_main(void) 
{
    call_global_constructors();
    create_heap();
    
    print_logo();
    malloc_test();
}

void print_logo()
{
    kout << R"(
     _  _____   ____  ___  
    | |/ / _ \ / ___|/ _ \ 
    | ' / | | | |  _| | | |
    | . \ |_| | |_| | |_| |
    |_|\_\___/ \____|\___/ 
                        
)";
}

void malloc_test()
{
    kout << "Setting up heap at " << alloc::k_HeapStart << "\n";
    kout << "Heap Size: " << static_cast<int>(alloc::k_HeapSize) << "\n";
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    kout << "allocating 10 integers 40 bytes" << '\n';
    
    int* arr { reinterpret_cast<int*>(malloc(sizeof(int) * 10)) };

    kout << "Size of Free block: " << static_cast<size_t>(sizeof(alloc::Block)) << "\n";
    kout << "Size of Allocation Header: " << static_cast<size_t>(sizeof(alloc::Header)) << "\n";
    for(int i{}; i < 10; ++i)
    {
        arr[i] = i;
        kout << arr[i] << " address: " << &arr[i] << "\n";
    }
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";

    int alloc_size = free_mem() - 8;
    auto rest = malloc(alloc_size);
    if(rest)
    {
        kout << "allocated " << alloc_size << " bytes" << "\n";
    }
    else
    {
        kout << "couldnt allocate the rest of the memory\n";
    }

    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";

    char* more { reinterpret_cast<char*>(malloc(1)) };
    if(!more)
    {
        kout << "couldnt find available memory\n";
    }
    else
    {
        kout << "allocated more\n";
    }
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
}