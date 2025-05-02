#include "idt.h"
#include "timer_handler.h"

extern "C" void __attribute__((cdecl)) timer_handler()
{
    // handle input from keyboard

    // send end of interrupt
    pic_send_eoi(0);
}