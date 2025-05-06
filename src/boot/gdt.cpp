#include "gdt.h"

GDTEntry gdt[3];

void set_gdt_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].limit_low     = limit & 0xFFFF;
    gdt[i].base_low      = base & 0xFFFF;
    gdt[i].base_middle   = (base >> 16) & 0xFF;
    gdt[i].access        = access;
    gdt[i].granularity   = (limit >> 16) & 0x0F;
    gdt[i].granularity  |= gran & 0xF0;
    gdt[i].base_high     = (base >> 24) & 0xFF;
}

extern "C" void setup_gdt() {
    set_gdt_entry(0, 0, 0, 0, 0);                // null segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment

    GDTPointer gdt_ptr;
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    asm volatile ("lgdt (%0)" : : "r" (&gdt_ptr));
    
    // Reload segment registers
    asm volatile (
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $next\n"
        "next:\n"
        : : : "memory"
    );
}
