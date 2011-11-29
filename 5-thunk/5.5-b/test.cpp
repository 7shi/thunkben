#include <functional>
#include <stdio.h>
#include "xbyak/xbyak.h"

template <class T>
struct Thunk : public Xbyak::CodeGenerator {
    Thunk(T *f) {
        mov(ecx, reinterpret_cast<intptr_t>(f));
        auto p = &T::operator();
        jmp(*reinterpret_cast<void **>(&p));
    }

    void (*getPtr())() {
        return reinterpret_cast<void (*)()>(
            const_cast<Xbyak::uint8 *>(getCode()));
    }
};

void call(void (*f)()) { f(); }

int main() {
    int a = 1;
    auto f = [&] { printf("%d\n", a++); };
    f();
    f();
    Thunk<decltype(f)> thunk(&f);
    auto f2 = thunk.getPtr();
    f2();
    call(f2);
}
