#pragma once

#include "i_kbrd_interruptable.h"
#include "kout.h"
#include "kin.h"
#include "idt.h"

class Shell : public IKbrdInterruptable
{
public:
    void interrupt(char c) override;
    void start();

private:
    bool show_prompt{ true };
};


extern Shell shell;