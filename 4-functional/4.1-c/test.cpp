#include <stdio.h>

void test(int a) {
    printf("%d\n", a);
}

struct Test {
    static void show(int n) {
        printf("%d\n", n);
    }
};

void call(void (*f)(int), int a) {
    f(a);
}

int main() {
    call(test, 1);
    call(Test::show, 1);
}
