#include "shell.h"

Shell shell{};

void Shell::interrupt(char c)
{
    if(c == '\0') return;

    kout << c;
    if(c == '\n' && show_prompt) kout << " > ";
}

void Shell::start()
{
    kout << "\n";
    
    KIn::InputString input;

    while(1)
    {
        kout.set_cursor_state(true);
        kout << " > ";

        show_prompt = false;
        kin.get_line(input);
        kout.set_cursor_state(false);
        kout << input.data() << "\n";
        show_prompt = true;
    }
}