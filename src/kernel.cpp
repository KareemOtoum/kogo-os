#include "kout.h"
#include "init.h"

extern "C" void kernel_main(void) 
{
    call_global_constructors();

    kout << "hello world" << '\n';
    kout << "we have integers! " << 234 << "\n";

    uint32_t num { 0xADB102BA };
    kout << "we have hex! " << num << "\n";

    uint32_t* num_ptr { &num };
    kout << "do we have ptrs? " << num_ptr;
}