#include "kout.h"
#include "init.h"

extern "C" void kernel_main(void) 
{
    call_global_constructors();

    kout << "hello world\n from kernel!\n";
}