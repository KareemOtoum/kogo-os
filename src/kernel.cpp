#include "kout.h"
#include "malloc.h"
#include "idt.h"
#include "new.h"
#include "keyboard_driver.h"
#include "shell.h"
#include "bitmap.h"

#include "pagedir.h"
#include "pagetable.h"

void malloc_test();
void print_logo();
void setup_pagetable();

extern "C" void kernel_main(void) 
{
    create_heap();
    init_idt();
    print_logo();
    
    init_bitmap();
    setup_pagetable();
    
    asm volatile("sti");

    shell.start();
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

void setup_pagetable()
{
    PageDirectory* first_page_dir { (PageDirectory*)phys::k_FirstPageDirAddr };
    PageTable* first_page_table { (PageTable*)phys::k_FirstPageTableAddr };

    memset(first_page_dir, 0, sizeof(PageDirectory));
    memset(first_page_table, 0, sizeof(PageTable));

    // present, read/write
    uint32_t flags = 0x3;
    for(int frame{}; frame < 1024; ++frame)
    {
        uint32_t physical_addr = (uint32_t)frame_to_address(frame);
        first_page_table->entries[frame] = (physical_addr & 0xFFFFF000) | flags;
    }

    // read write, present
    first_page_dir->entries[0] = ((uint32_t)first_page_table & 0xFFFFF000) | flags;

    // enable paging
    asm volatile ("mov %0, %%cr3" :: "r"(first_page_dir));
    // setting PG bit 31
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));         // Read CR0 into cr0
    cr0 |= 0x80000000;                                  // Set PG bit (bit 31)
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));         // Write back to CR0

    volatile uint32_t* test_addr = (volatile uint32_t*)0x200000; // any mapped address
    *test_addr = 0xDEADBEEF;
    uint32_t read_back = *test_addr;

    if (read_back == 0xDEADBEEF) 
    {
        kout << "Setup page table with kernel pages\n";
    }
}