#include "kin.h"

KIn kin{};

static std::array<char, input::k_BufferSize> buffer{};

static int buffer_index { 0 };
static bool flush { false };

void KIn::interrupt(char c)
{
    if(c == '\0') return;

    if(c == '\n') flush = true;
    else if(buffer_index < input::k_BufferSize - 1) // space for null terminator
    {
        buffer[buffer_index++] = c;
    }
    else
    {
        // buffer is full
        memset(buffer.data(), 0, sizeof(buffer));
        buffer_index = 0;
    }
}

void KIn::get_line(std::array<char, input::k_BufferSize>& str)
{
    memset(buffer.data(), 0, sizeof(buffer));
    buffer_index = 0;

    while(!flush) {};

    // copy buffer into string
    memcpy(buffer.data(), str.data(), buffer_index + 1);
    // add null terminator
    str[buffer_index] = '\0';

    // clear buffer
    buffer_index = 0;
    memset(buffer.data(), 0, sizeof(buffer));

    flush = false;
}