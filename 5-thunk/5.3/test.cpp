#include <stdio.h>
#include "xbyak/xbyak.h"

template <class T, class TA1>
struct Thunk : public Xbyak::CodeGenerator {
    Thunk(T *t, void (T::*p)(TA1)) {
        mov(ecx, reinterpret_cast<intptr_t>(t));
        push(ptr[esp+4]);
        call(*reinterpret_cast<void **>(&p));
        ret();
    }

    void (*getPtr())(TA1) {
        return reinterpret_cast<void (*)(TA1)>(
            const_cast<Xbyak::uint8 *>(getCode()));
    }
};

struct Test {
    int n;
    Test(int n) : n(n) {}
    void show(int n2) {
        printf("%d, %d\n", n, n2);
    }
};

void show(int n) {
    printf("%d\n", n);
}

void call(void (*f)(int), int n) { f(n); }

int main() {
    Test t(1);
    Thunk<Test, int> thunk(&t, &Test::show);
    call(thunk.getPtr(), 2);
    call(show, 3);
}
