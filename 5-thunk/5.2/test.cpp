#include <stdio.h>
#include "xbyak/xbyak.h"

template <class T>
struct Delegate : public Xbyak::CodeGenerator {
    Delegate(T *t, void (T::*p)()) {
        mov(ecx, reinterpret_cast<intptr_t>(t));
        jmp(*reinterpret_cast<void **>(&p)); // Å¶
    }

    void (*getPtr())() {
        return reinterpret_cast<void (*)()>(
            const_cast<Xbyak::uint8 *>(getCode()));
    }
};

struct Test1 {
    int n;
    Test1(int n) : n(n) {}
    void show() {
        printf("%d\n", n);
    }
};

struct Test2 {
    int n;
    Test2(int n) : n(n) {}
    void show() {
        printf("%d\n", n);
    }
};

struct Test3 {
    static void show() {
        printf("?\n");
    }
};

void show() {
    printf("!\n");
}

void call(void (*f)()) { f(); }

int main() {
    Test1 t1(1);
    Test2 t2(2);
    Delegate<Test1> d1(&t1, &Test1::show);
    Delegate<Test2> d2(&t2, &Test2::show);
    call(d1.getPtr());
    call(d2.getPtr());
    call(Test3::show);
    call(show);
}
