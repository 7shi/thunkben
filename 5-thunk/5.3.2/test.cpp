#include <stdio.h>
#include "xbyak/xbyak.h"

struct Conv : public Xbyak::CodeGenerator {
    Conv(int(_stdcall *p)(int, int)) {
        push(ptr[esp+8]);
        push(ptr[esp+8]);
        call(p);
        ret();
    }
};

void call(int(*p)(int, int), int a, int b) {
    printf("%d\n", p(a, b));
}

int _stdcall test(int a, int b) { return a + b; }

int main() {
    Conv c(test);
    auto f = reinterpret_cast<int(*)(int, int)>(
        const_cast<Xbyak::uint8 *>(c.getCode()));
    call(f, 2, 3);
}
