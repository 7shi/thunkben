#include <functional>
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

void call(const std::function<void()> &f) {
    f();
}

int main() {
    Test1 t1(1);
    Test2 t2(2);
    auto f1 = std::bind(&Test1::show, &t1);
    auto f2 = [&] { t2.show(); };
    auto f3 = Test3::show;
    auto f4 = show;
    call(f1);
    call(f2);
    call(f3);
    call(f4);
}
