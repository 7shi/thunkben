#include <stdio.h>

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
    auto f1 = &Test1::show;
    auto f2 = &Test2::show;
    auto f3 = &Test3::show;
    auto f4 = &show;
    (t1.*f1)();
    (t2.*f2)();
    (*f3)();
    (*f4)();
    call(f3);
    call(f4);
}
