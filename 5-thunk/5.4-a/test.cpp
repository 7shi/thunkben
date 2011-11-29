#include <stdio.h>
#include "xbyak/xbyak.h"

struct Bind : public Xbyak::CodeGenerator {
    Bind(int(*f)(int, int), int a) {
        push(a);
        push(ptr[esp+8]);
        call(reinterpret_cast<void *>(f));
        add(esp, 8);
        ret();
    }

    int(*getPtr())(int) {
        return reinterpret_cast<int(*)(int)>(
            const_cast<Xbyak::uint8 *>(getCode()));
    }
};

int add(int a, int b) { return a + b; }

void call(int(*p)(int), int a) {
    printf("%d\n", p(a));
}

int main() {
    Bind inc(add, 1);
    call(inc.getPtr(), 2);
}
