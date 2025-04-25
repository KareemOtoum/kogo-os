#include "init.h"

typedef void (*constructor_t)();

extern "C" constructor_t __CTOR_LIST__;
extern "C" constructor_t __CTOR_END__;

extern "C" void call_global_constructors() {
    for (constructor_t* ctor = &__CTOR_LIST__; ctor < &__CTOR_END__; ++ctor) {
        (*ctor)();
    }
}
