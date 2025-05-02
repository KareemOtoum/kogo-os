#include "idt.h"

static IDTEntry idt[256]{};
static IDTPointer idt_ptr;

void set_entry(int vector, void (*handler)(), uint16_t selector, uint8_t attr)
{
    uint32_t func_handler { reinterpret_cast<uint32_t>(handler) };

    idt[vector].offset_low = func_handler & 0xFFFF;
    idt[vector].offset_high = (func_handler >> 16) & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].zero = 0;
    idt[vector].attr = attr;
}

void io_wait() {
    outb(0x80, 0); // Port 0x80 is often unused; this adds a slight delay
}

void pic_remap(int offset1, int offset2) {
    // Save masks
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);

    // Start initialization of both PICs
    outb(0x20, 0x11); // Start init of master PIC
    io_wait();
    outb(0xA0, 0x11); // Start init of slave PIC
    io_wait();

    // Set new vector offsets
    outb(0x21, offset1); // Master PIC vector offset (e.g., 0x20)
    io_wait();
    outb(0xA1, offset2); // Slave PIC vector offset (e.g., 0x28)
    io_wait();

    // Tell master PIC about the slave PIC at IRQ2
    outb(0x21, 0x04); // 0000 0100 = IRQ2 has slave
    io_wait();
    outb(0xA1, 0x02); // Slave identity = cascade on IRQ2
    io_wait();

    // Set PICs to 8086/88 (x86) mode
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    // mask all interrupts
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    // Clear interrupt masks for the IRQs we want to use
    outb(0x21, ~0x03); // Only enable IRQ0 (timer) and IRQ1 (keyboard)
    outb(0xA1, 0xFF);  // Mask all slave IRQs
}

extern "C" void pic_send_eoi(unsigned char irq)
{
    if(irq >= 8)
        outb(0xA0, 0x20); // slave
    outb(0x20, 0x20);     // master
}

void init_idt()
{

    pic_remap(0x20, 0x28);
    
    set_entry(0x21, isr33, 0x08, 0x8E); // Keyboard IRQ1
    set_entry(0x20, isr32, 0x08, 0x8E); // Timer IRQ0
    
    idt_ptr.base = reinterpret_cast<uint32_t>(&idt);
    idt_ptr.limit = sizeof(idt) - 1;
    lidt(&idt_ptr);

}

