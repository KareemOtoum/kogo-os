#include "mem.h"

extern "C" void memset(void* buffer, int val, int size)
{
    char* byte = reinterpret_cast<char*>(buffer);

    for(int i{}; i < size; ++i)
    {
        *byte = val;
        ++byte;
    }
}

extern "C" void memcpy(void* in, void* out, int size)
{
    char* byte_in = reinterpret_cast<char*>(in);
    char* byte_out = reinterpret_cast<char*>(out);

    for(int i{}; i < size; ++i)
    {
        *byte_out = *byte_in;
        ++byte_in;
        ++byte_out;
    }
}