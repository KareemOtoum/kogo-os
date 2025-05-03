#include "shell.h"

Shell shell{};

void Shell::interrupt(char c)
{
    if(c == '\0') return;

    kout << c;
    if(c == '\n') kout << " > ";
}

void Shell::start()
{
    kout << "\n";
    kout.set_cursor_state(true);

    kout << " > ";
}

void Shell::stop()
{
    kout.set_cursor_state(false);
    kout << "\n";
}