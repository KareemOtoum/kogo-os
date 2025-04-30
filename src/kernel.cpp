#include "kout.h"
#include "malloc.h"
#include "init.h"

void malloc_test();
void print_logo();

extern "C" void kernel_main(void) 
{
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
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    kout << "allocating 10 integers 40 bytes" << '\n';
    
    int* arr { reinterpret_cast<int*>(malloc(sizeof(int) * 10)) };
    log_freelist();

    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    
    kout << "Freeing integer array of 10 elements\n";
    free(arr);
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    log_freelist();

    kout << "Allocating " << static_cast<int>(free_mem() - 48) << "\n";

    char* arr2 { reinterpret_cast<char*>(malloc(free_mem() - 48)) };
    if(arr2)
    {
        kout << "successfully allocated\n";
    }
    else
    {
        kout << "allocation failed\n";
    }
    log_freelist();
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";

    kout << "Freeing\n";
    free(arr2);
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    log_freelist();


}