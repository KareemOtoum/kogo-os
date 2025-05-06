#include "init.h"

typedef void (*constructor_t)();

extern "C" constructor_t __ctors_start;
extern "C" constructor_t __ctors_end;

extern "C" void call_global_constructors() {
    for (constructor_t* ctor = &__ctors_start; ctor < &__ctors_end; ++ctor) {
        (*ctor)();
    }
}
