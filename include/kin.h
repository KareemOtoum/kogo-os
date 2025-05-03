#pragma once
#include "i_kbrd_interruptable.h"

class KIn : public IKbrdInterruptable
{
public:
    void interrupt(char c) override
    {

    }

private:
};

extern KIn kin;