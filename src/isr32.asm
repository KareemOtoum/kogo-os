.global isr32
.extern timer_handler

isr32:
    cli
    pushal
    call timer_handler
    popal
    iret
