#include "new.h"

void* operator new(std::size_t size)
{
    if(void* ptr = malloc(size))
    {
        return ptr;
    }
    else
    {
        kout << "Bad allocation\n";
        return nullptr;
    }
}

void operator delete(void* ptr) noexcept
{
    free(ptr);
}

void* operator new[](std::size_t size)
{
    kout << "calling new[]\n";
    if(void* ptr = malloc(size))
    {
        return ptr;
    }
    else
    {
        kout << "Bad allocation\n";
        return nullptr;
    }
}

void operator delete[](void* ptr) noexcept
{
    kout << "calling delete[]\n";
    free(ptr);
}