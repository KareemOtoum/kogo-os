.global isr33
.extern keyboard_handler

isr33:
    cli
    pushal
    call keyboard_handler
    popal
    iret
