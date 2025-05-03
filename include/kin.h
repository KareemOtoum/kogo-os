#pragma once

#include <array>
#include "i_kbrd_interruptable.h"
#include "mem.h"
#include "kout.h"

namespace input
{
    constexpr int k_BufferSize { 256 };
}

class KIn : public IKbrdInterruptable
{
public:
    using InputString = std::array<char, input::k_BufferSize>;

    void interrupt(char c) override;
    void get_line(InputString& str);

private:
};

extern KIn kin;