
.global isr14
.extern pfault_handler
isr14:
    cli
    pushl $0       # for alignment
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi
    pushl %ebp
    pushl %esp
    pushl %ss

    call pfault_handler

    addl $60, %esp 
    sti
    iret
