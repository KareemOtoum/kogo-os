# 🐨 KOGO

![Screenshot 2025-04-29 013203](https://github.com/user-attachments/assets/1289168d-11ea-490a-a720-329b3f9eea40)

A simple educational 32-bit x86 operating system built from scratch using C++ and no STL. 
This project explores core low-level concepts including, virtual memory, processes and scheduling, 
interrupt handling, kernel memory management, and VGA text output.  

## 🧠 Features  

- ✅ **Free list `malloc` for dynamic kernel memory allocation**
- ✅ **Flat memory model**
- ✅ **VGA text-mode with cout-like driver interface**
- ✅ **Basic shell with keyboard input via hardware interrupts**
- ✅ **Physical memory page allocator using a bitmap**
- ✅ **Virtual memory with paging (4 KiB pages)**
- ✅ **Grub Bootloader**  

## 🚧 Roadmap  
- Processes and scheduling
- More complete shell and basic system calls
- User-mode support and privilege switching  


## 🧰 Build & Run

### 🔨 Requirements

- GCC 14.2.0 cross-compiler for `i686-elf-gcc` with c++ enabled
- GCC binutils 2.44 
- QEMU

the cross compiler can be built by following this tutorial: https://wiki.osdev.org/GCC_Cross-Compiler

### 🚀 Build

```bash
git clone https://github.com/KareemOtoum/kogo-os
cd kogo-os
make run
```

📚 Inspiration & Resources
 - OSDev Wiki
 - Barebones OSDev Guide
 - Intel® 80386 Programmer's Reference Manual
