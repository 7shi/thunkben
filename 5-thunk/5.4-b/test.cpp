#include <functional>
#include <stdio.h>
#include "xbyak/xbyak.h"

template <class T, class TA1, class TR>
struct Thunk : public Xbyak::CodeGenerator {
    Thunk(T *f) {
        mov(ecx, reinterpret_cast<intptr_t>(f));
        push(ptr[esp+4]);
        auto p = &T::operator();
        call(*reinterpret_cast<void **>(&p));
        ret();
    }

    TR(*getPtr())(TA1) {
        return reinterpret_cast<TR(*)(TA1)>(
            const_cast<Xbyak::uint8 *>(getCode()));
    }
};

int add(int a, int b) { return a + b; }

void call(int(*p)(int), int a) {
    printf("%d\n", p(a));
}

int main() {
    std::function<int(int)> inc =
        std::bind(add, std::placeholders::_1, 1);
    Thunk<decltype(inc), int, int> thunk(&inc);
    call(thunk.getPtr(), 2);
}
