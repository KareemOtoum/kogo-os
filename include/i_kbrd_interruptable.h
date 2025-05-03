#pragma once
#include <cstdint>

class IKbrdInterruptable
{
public:
    virtual void interrupt(char c) = 0;
};