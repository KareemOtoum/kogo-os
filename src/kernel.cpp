#include "kout.h"
#include "malloc.h"
#include "idt.h"
#include "new.h"

void malloc_test();
void print_logo();

extern "C" void kernel_main(void) 
{
    create_heap();
    print_logo();
    init_idt();
    
    kout << "Enabling interrupts...\n";
    
    // Enable interrupts
    asm volatile("sti");
    
    kout << "Interrupts enabled. Waiting for events...\n";
    
    while(1) {
        asm volatile("hlt");  // Halt until next interrupt
    }// Keep CPU busy so we can receive interrupts
    
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
    
    int* arr { new int[10] };
    log_freelist();

    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    
    kout << "Freeing integer array of 10 elements\n";

    delete[] arr;

    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    log_freelist();

    kout << "Allocating " << static_cast<int>(free_mem() - 48) << "\n";

    char* arr2 { new char[free_mem() - 48] };

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

    kout << "Freeing array\n";
    delete[] arr2;
    kout << "Free memory: " << static_cast<int>(free_mem()) << "\n";
    log_freelist();
}