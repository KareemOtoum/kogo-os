#include "keyboard_handler.h"

static std::array<IKbrdInterruptable*, 2> interruptables 
{ 
    reinterpret_cast<IKbrdInterruptable*>(&kin), 
    reinterpret_cast<IKbrdInterruptable*>(&shell)
};

extern "C" void  __attribute__((cdecl)) keyboard_handler()
{
    uint8_t scancode = inb(0x60);

    // handle input from keyboard

    char c { kbrd::translate(scancode) };

    for(auto interruptable : interruptables)
    {
        interruptable->interrupt(c);
    }

    // send end of interrupt
    pic_send_eoi(1);
}