#include "keyboard_handler.h"

extern "C" void  __attribute__((cdecl)) keyboard_handler()
{
    uint8_t scancode = inb(0x60);

    // handle input from keyboard
    kout << "Handling irq33 interrupt\n";

    // send end of interrupt
    pic_send_eoi(1);
}