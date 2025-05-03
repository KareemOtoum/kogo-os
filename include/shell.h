#pragma once

#include "i_kbrd_interruptable.h"
#include "kout.h"
#include "idt.h"

class Shell : public IKbrdInterruptable
{
public:
    void interrupt(char c) override;
    void start();
    void stop();

private:

};


extern Shell shell;