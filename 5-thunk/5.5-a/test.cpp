#include <functional>
#include <stdio.h>
#include "xbyak/xbyak.h"

void call(void (*f)()) { f(); }

int main() {
    int a = 1;
    struct _ : public Xbyak::CodeGenerator {
        _(int *a) {
            push(reinterpret_cast<intptr_t>(a));
            call(reinterpret_cast<void *>(run));
            add(esp, 4);
            ret();
        }

        static void run(int &a) {
            printf("%d\n", a++);
        }
    } closure(&a);
    auto f = reinterpret_cast<void (*)()>(
        const_cast<Xbyak::uint8 *>(closure.getCode()));
    f();
    f();
    call(f);
}
