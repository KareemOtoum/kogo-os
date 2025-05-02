#pragma once
#include <cstdint>
#include "kout.h"

struct __attribute__((packed)) IDTEntry
{
    uint16_t offset_low; // lower 16 bits of handler func
    uint16_t selector; // kernel code seg selector
    uint8_t zero; // always zero
    uint8_t attr; // type and attributes
    uint16_t offset_high; // uper 16 bits of handler func
} ; 

struct __attribute__((packed)) IDTPointer 
{
    uint16_t limit;
    uint32_t base;
};

extern "C" inline void lidt(void* idt_ptr)
{
    asm volatile("lidtl (%0)" : : "r"(idt_ptr));
}

void set_entry(int vector, void (*handler)(), uint16_t selector, uint8_t attr);
extern "C" void init_idt();


inline void outb(uint16_t port, uint8_t val) 
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline uint8_t inb(uint16_t port) 
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

extern "C" void pic_send_eoi(unsigned char irq);

// handlers
extern "C" void isr33(); // asm keyboard handler
extern "C" void isr32(); // asm timer int handler