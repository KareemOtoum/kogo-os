# 🐨 KOGO

![Screenshot 2025-04-29 013203](https://github.com/user-attachments/assets/1289168d-11ea-490a-a720-329b3f9eea40)

A simple educational 32-bit x86 operating system built from scratch using C++ and no STL. 
This project explores core low-level concepts including, virtual memory, processes and scheduling, 
interrupt handling, kernel memory management, and VGA text output.  

## 🧠 Features  

- ✅ **32-bit x86 architecture**
- ✅ **Free list `kmalloc` for dynamic kernel memory allocation**
- ✅ **Flat memory model**
- ✅ **VGA text-mode kernel output**
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

- GCC cross-compiler for i386 (`i686-elf-gcc`) with c++ enabled
- GCC binutils and ld
- QEMU

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
