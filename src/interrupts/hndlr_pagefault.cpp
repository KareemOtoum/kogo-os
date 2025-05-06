#include "idt.h"
#include "kout.h"

extern "C" void pfault_handler(uint32_t error_code)
{
    uint32_t fault_addr;
    asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

    kout << "Page fault at address: " << (void*)fault_addr << "\n";
}
