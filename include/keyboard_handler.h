#pragma once

#include <cstdint>
#include "idt.h"
#include "kout.h"
#include "kin.h"
#include "shell.h"
#include "keyboard_driver.h"
#include "i_kbrd_interruptable.h"
#include <array>

extern "C" void keyboard_handler();